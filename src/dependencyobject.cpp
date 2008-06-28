/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * dependencyobject.h: 
 *
 * Copyright 2007 Novell, Inc. (http://www.novell.com)
 *
 * See the LICENSE file included with the distribution for details.
 * 
 */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "debug.h"
#include "namescope.h"
#include "list.h"
#include "collection.h"
#include "dependencyobject.h"
#include "clock.h"
#include "runtime.h"
#include "uielement.h"
#include "animation.h"


#ifdef DEBUG
#define d(x) x
#else
#define d(x)
#endif


EventLists::EventLists (int n)
{
	size = n;
	emitting = 0;
	lists = new EventList [size];
	for (int i = 0; i < size; i++) {
		lists [i].current_token = 0;
		lists [i].emitting = 0;
		lists [i].event_list = new List ();
	}
}

EventLists::~EventLists ()
{
	for (int i = 0; i < size; i++) {
		delete lists [i].event_list;
	}
	delete [] lists;
}

/*
 *
 */

EventObject::EventObject ()
{
	surface = NULL;
	refcount = 1;
	events = NULL;

#if OBJECT_TRACKING
	id = ++objects_created;
	if (objects_alive == NULL)
		objects_alive = g_hash_table_new (g_direct_hash, g_direct_equal);
	// Helgrind correctly reports a race here. Given this is for debugging, ignore it.
	g_hash_table_insert (objects_alive, this, GINT_TO_POINTER (1));
	weak_refs = NULL;

	Track ("Created", "");
#elif DEBUG
	objects_created++;
#endif
}

EventObject::~EventObject()
{
#if OBJECT_TRACKING
	objects_destroyed++;
	g_hash_table_remove (objects_alive, this);
	if (weak_refs) {
		int length = g_hash_table_size (weak_refs);
		if (length > 0) {
			printf ("Destroying id=%i with %i weak refs.\n", id, length);
			GList* list = g_hash_table_get_values (weak_refs);
			GList* first = list;
			while (list != NULL) {
				EventObject* eo = (EventObject*) list->data;
				printf ("\t%s %i.\n", eo->GetTypeName (), eo->id);
				list = list->next;
			}
			g_list_free (first);
		}
		g_hash_table_destroy (weak_refs);
		weak_refs = NULL;
	}

	if (refcount != 0) {
		printf ("Object #%i was deleted before its refcount reached 0 (current refcount: %i)\n", id, refcount);
		//print_stack_trace ();
	}

	Track ("Destroyed", "");
#elif DEBUG
	objects_destroyed++;
#endif

	delete events;
}

static pthread_rwlock_t surface_lock = PTHREAD_RWLOCK_INITIALIZER;

bool
EventObject::SetSurfaceLock ()
{
	int result;
	
	if ((result = pthread_rwlock_wrlock (&surface_lock)) != 0) {
		printf ("EventObject::SetSurface (%p): Couldn't aquire write lock: %s\n", surface, strerror (result));
		return false;
	}
	
	return true;
}


void
EventObject::SetSurface (Surface *surface)
{
	if (!Surface::InMainThread ()) {
		g_warning ("EventObject::SetSurface (): This method must not be called on any other than the main thread!\n");
		d (print_stack_trace ());
		return;
	}
	
	this->surface = surface;
}

void
EventObject::SetSurfaceUnlock ()
{
	pthread_rwlock_unlock (&surface_lock);
}

void
EventObject::AddTickCallSafe (TickCallHandler handler)
{
	int result;
	
	/*
	 * This code assumes that the surface won't be destructed without setting the surface field on to NULL first. 
	 * In other words: if we have a read lock here, the surface won't get destroyed, given that setting
	 * the surface field to NULL will block until we release the read lock.
	 */
	
	if ((result = pthread_rwlock_rdlock (&surface_lock)) != 0) {
		printf ("EventObject::AddTickCallSafe (): Couldn't aquire read lock: %s\n", strerror (result));
		return;
	}

	AddTickCallInternal (handler);
 	
	pthread_rwlock_unlock (&surface_lock);
}

void
EventObject::AddTickCall (TickCallHandler handler)
{
	if (!Surface::InMainThread ()) {
		g_warning ("EventObject::AddTickCall (): This method must not be called on any other than the main thread! Tick call won't be added.\n");
		d (print_stack_trace ());
		return;
	}
	
	AddTickCallInternal (handler);
}

void
EventObject::AddTickCallInternal (TickCallHandler handler)
{
	Surface *surface;
	TimeManager *timemanager;
	
	surface = GetSurface ();
	
	if (!surface) {
		d(printf ("EventObject::AddTickCall (): Could not add tick call, no surface\n"));
		return;
	}
	
	timemanager = surface->GetTimeManager ();
	
	if (!timemanager) {
		d(printf ("EventObject::AddTickCall (): Could not add tick call, no time manager\n"));
		return;
	}

	ref ();
	timemanager->AddTickCall (handler, this);
}

void 
EventObject::unref ()
{
	bool delete_me;

	if (!Surface::InMainThread ()) {
		unref_delayed ();
		return;
	}

	if (refcount == 1 && events != NULL && events->emitting) {
		unref_delayed ();
		return;
	}

	if (refcount == 1)
		Emit (DestroyedEvent);

	delete_me = g_atomic_int_dec_and_test (&refcount);

	OBJECT_TRACK ("Unref", GetTypeName ());

	if (delete_me) {
		SetSurface (NULL);
		delete this;
	}
}

#if DEBUG
int EventObject::objects_created = 0;
int EventObject::objects_destroyed = 0;
#endif

#if OBJECT_TRACKING
// Define the ID of the object you want to track
// Object creation, destruction and all ref/unrefs
// are logged to the console, with a stacktrace.
static bool object_id_fetched = false;
static int object_id = -1;

#define OBJECT_TRACK_ID (0)

GHashTable* EventObject::objects_alive = NULL;

void
EventObject::Track (const char* done, const char* typname)
{
	if (!object_id_fetched) {
		object_id_fetched = true;
		char *sval = getenv ("MOONLIGHT_OBJECT_TRACK_ID");
		if (sval)
			object_id = atoi (sval);
	}
	if (id == object_id) {
		char *st = get_stack_trace ();
		printf ("%s tracked object of type '%s': %i, current refcount: %i\n%s", done, typname, id, refcount, st);
		g_free (st);
	}
}

void
EventObject::weak_ref (EventObject* base)
{
	if (weak_refs == NULL)
		weak_refs = g_hash_table_new (g_direct_hash, g_direct_equal);
	g_hash_table_insert (weak_refs, base, base);
}

void
EventObject::weak_unref (EventObject* base)
{
	// if you get a glib assertion here, you're most likely accessing an already 
	// destroyed object (or you have mismatched ref/unrefs).
	g_hash_table_remove (weak_refs, base);
}

char *
EventObject::GetStackTrace (const char* prefix)
{
	return get_stack_trace_prefix (prefix);
}

void
EventObject::PrintStackTrace ()
{
	print_stack_trace ();
}
#endif

// event handlers for c++
class EventClosure : public List::Node {
public:
	EventClosure (EventHandler func, gpointer data, int token) {
		this->func = func;
		this->data = data;
		this->token = token;

		pending_removal = false;
		emit_count = 0;
	}
	

	EventHandler func;
	gpointer data;
	int token;
	bool pending_removal;
	int emit_count;
};

int
EventObject::AddHandler (const char *event_name, EventHandler handler, gpointer data)
{
	int id = GetType()->LookupEvent (event_name);

	if (id == -1) {
		g_warning ("adding handler to event '%s', which has not been registered\n", event_name);
		return -1;
	}

	return AddHandler (id, handler, data);
}

int
EventObject::AddHandler (int event_id, EventHandler handler, gpointer data)
{ 
	if (GetType()->GetEventCount() <= 0) {
		g_warning ("adding handler to event with id %d, which has not been registered\n", event_id);
		return -1;
	}
	
	if (events == NULL)
		events = new EventLists (GetType ()->GetEventCount ());
	
	int token = events->lists [event_id].current_token++;
	
	events->lists [event_id].event_list->Append (new EventClosure (handler, data, token));
	
	return token;
}


void
EventObject::RemoveHandler (const char *event_name, EventHandler handler, gpointer data)
{
	int id = GetType()->LookupEvent (event_name);

	if (id == -1) {
		g_warning ("removing handler for event '%s', which has not been registered\n", event_name);
		return;
	}

	RemoveHandler (id, handler, data);
}

void
EventObject::RemoveHandler (const char *event_name, int token)
{
	int id = GetType()->LookupEvent (event_name);

	if (id == -1) {
		g_warning ("removing handler for event '%s', which has not been registered\n", event_name);
		return;
	}

	RemoveHandler (id, token);
}

void
EventObject::RemoveHandler (int event_id, EventHandler handler, gpointer data)
{
	if (GetType()->GetEventCount() <= 0) {
		g_warning ("removing handler for event with id %d, which has not been registered\n", event_id);
		return;
	}
	
	if (events == NULL)
		return;
	
	EventClosure *closure = (EventClosure *) events->lists [event_id].event_list->First ();
	while (closure) {
		if (closure->func == handler && closure->data == data) {
 			if (events->lists [event_id].emitting > 0) {
 				closure->pending_removal = true;
 			} else {
				events->lists [event_id].event_list->Remove (closure);
 			}
			break;
		}
		
		closure = (EventClosure *) closure->next;
	}
}

void
EventObject::RemoveHandler (int event_id, int token)
{
	if (GetType()->GetEventCount() <= 0) {
		g_warning ("removing handler for event with id %d, which has not been registered\n", event_id);
		return;
	}
	
	if (events == NULL)
		return;
	
	EventClosure *closure = (EventClosure *) events->lists [event_id].event_list->First ();
	while (closure) {
		if (closure->token == token) {
			if (events->lists [event_id].emitting > 0) {
				closure->pending_removal = true;
			} else {
				events->lists [event_id].event_list->Remove (closure);
			}
			break;
		}
		
		closure = (EventClosure *) closure->next;
	}
}

void
EventObject::RemoveMatchingHandlers (const char *event_name, bool (*predicate)(EventHandler cb_handler, gpointer cb_data, gpointer data), gpointer closure)
{
	int id = GetType()->LookupEvent (event_name);

	if (id == -1) {
		g_warning ("removing handler for event '%s', which has not been registered\n", event_name);
		return;
	}

	RemoveMatchingHandlers (id, predicate, closure);
}

void
EventObject::RemoveMatchingHandlers (int event_id, bool (*predicate)(EventHandler cb_handler, gpointer cb_data, gpointer data), gpointer closure)
{
	if (GetType()->GetEventCount() <= 0) {
		g_warning ("removing handler for event with id %d, which has not been registered\n", event_id);
		return;
	}
	
	if (events == NULL)
		return;

	EventClosure *c = (EventClosure *) events->lists [event_id].event_list->First ();
	while (c) {
		if (predicate (c->func, c->data, closure)) {
			if (events->lists [event_id].emitting > 0) {
				c->pending_removal = true;
			} else {
				events->lists [event_id].event_list->Remove (c);
			}
			break;
		}
		
		c = (EventClosure *) c->next;
	}
}

bool
EventObject::Emit (char *event_name, EventArgs *calldata, bool only_unemitted)
{
	int id = GetType()->LookupEvent (event_name);

	if (id == -1) {
		g_warning ("trying to emit event '%s', which has not been registered\n", event_name);
		return false;
	}

	return Emit (id, calldata, only_unemitted);
}

//#define DEBUG_EVENTS

bool
EventObject::Emit (int event_id, EventArgs *calldata, bool only_unemitted)
{
#ifdef DEBUG_EVENTS
	const char *event_name = GetType ()->LookupEventName (event_id);
	if (GetObjectType () == Type::MEDIAELEMENT)
	printf ("%s::Emit (%s (%i), %p, %i). Listeners: %i\n", GetTypeName (), event_name, event_id, calldata, only_unemitted, events == NULL ? 0 : events->lists [event_id].event_list->Length ());
#endif

	if (GetType()->GetEventCount() <= 0 || event_id >= GetType()->GetEventCount()) {
		g_warning ("trying to emit event with id %d, which has not been registered\n", event_id);
		if (calldata)
			calldata->unref ();
		return false;
	}

	if (events == NULL || events->lists [event_id].event_list->IsEmpty ()) {
		if (calldata)
			calldata->unref ();
		return false;
	}

	EmitContext* ctx = StartEmit (event_id);

	DoEmit (event_id, ctx, calldata, only_unemitted);

	if (calldata)
		calldata->unref ();

	FinishEmit (event_id, ctx);

	return true;
}

struct EmitContext {
  int length;
  EventClosure **closures;

  EmitContext()
  {
    length = 0;
    closures = NULL;
  }
  ~EmitContext()
  {
    g_free (closures);
  }
};

EmitContext*
EventObject::StartEmit (int event_id)
{
	EmitContext *ctx = new EmitContext();
	EventClosure *closure;

	if (GetType()->GetEventCount() <= 0 || event_id >= GetType()->GetEventCount()) {
		g_warning ("trying to start emit with id %d, which has not been registered\n", event_id);
		return ctx;
	}

	if (events == NULL || events->lists [event_id].event_list->IsEmpty ()) {
		return ctx;
	}

	events->emitting++;
	events->lists [event_id].emitting++;

	ctx->length = events->lists [event_id].event_list->Length();
	ctx->closures = g_new (EventClosure*, ctx->length);

	/* make a copy of the event list to use for emitting */
	closure = (EventClosure *) events->lists [event_id].event_list->First ();
	for (int i = 0; closure != NULL; i++) {
		ctx->closures [i] = closure;
		closure = (EventClosure *) closure->next;
	}

	return ctx;
}

bool
EventObject::DoEmit (int event_id, EmitContext *ctx, EventArgs *calldata, bool only_unemitted)
{
	/* emit the events using the copied list */
	for (int i = 0; i < ctx->length; i++) {
		EventClosure *closure = ctx->closures[i];
		if (closure && closure->func
		    && (!only_unemitted || closure->emit_count == 0)) {
			closure->func (this, calldata, closure->data);
			closure->emit_count ++;
		}
	}

	return ctx->length > 0;
}

void
EventObject::FinishEmit (int event_id, EmitContext *ctx)
{
	if (GetType()->GetEventCount() <= 0 || event_id >= GetType()->GetEventCount()) {
		g_warning ("trying to finish emit with id %d, which has not been registered\n", event_id);
		goto delete_ctx;
	}

	if (events == NULL || events->lists [event_id].event_list->IsEmpty ()) {
		goto delete_ctx;
	}

	events->lists [event_id].emitting--;
	events->emitting--;

	if (events->lists [event_id].emitting == 0) {
		// Remove closures which are waiting for removal
		EventClosure *closure = (EventClosure *) events->lists [event_id].event_list->First ();
		while (closure != NULL) {
			EventClosure *next = (EventClosure *) closure->next;
			if (closure->pending_removal)
				events->lists [event_id].event_list->Remove (closure);
			closure = next;
		}
	}

 delete_ctx:
	// make sure to pass false here, as the lists are shallow copied and the nodes are shared
	delete ctx;
}

Surface*
event_object_get_surface (EventObject *o)
{
	return o->GetSurface ();
}

void
event_object_add_event_handler (EventObject *o, const char *event, EventHandler handler, gpointer closure)
{
	o->AddHandler (event, handler, closure);
}

void
event_object_remove_event_handler (EventObject *o, const char *event, EventHandler handler, gpointer closure)
{
	o->RemoveHandler (event, handler, closure);
}

void 
base_ref (EventObject *obj)
{
	if (obj != NULL)
		obj->ref ();
}

void
base_unref (EventObject *obj)
{
	if (obj != NULL)
		obj->unref ();
}

static pthread_mutex_t delayed_unref_mutex = PTHREAD_MUTEX_INITIALIZER;
static bool drain_tick_call_added = false;
static GSList *pending_unrefs = NULL;

static gboolean
drain_unrefs_idle_call (gpointer data)
{
	drain_unrefs ();
	return false;
}

void
drain_unrefs ()
{
	GSList *list;

	// We need to unlock our mutex before unreffing the objects,
	// since unreffing any object might cause unref_delayed to be
	// called (on the same thread), which will then try to lock the
	// mutex again, causing a dead-lock.
	pthread_mutex_lock (&delayed_unref_mutex);
	list = pending_unrefs;
	pending_unrefs = NULL;
	drain_tick_call_added = false;
	pthread_mutex_unlock (&delayed_unref_mutex);

	g_slist_foreach (list, (GFunc) base_unref, NULL);
	g_slist_free (list);
}

void
EventObject::unref_delayed ()
{
	OBJECT_TRACK ("DelayedUnref", GetTypeName ());

	pthread_mutex_lock (&delayed_unref_mutex);
	pending_unrefs = g_slist_prepend (pending_unrefs, this);

	if (!drain_tick_call_added) {
		g_idle_add (drain_unrefs_idle_call, NULL);
		drain_tick_call_added = true;
	}
	pthread_mutex_unlock (&delayed_unref_mutex);
}


GHashTable *DependencyObject::properties = NULL;

typedef struct {
	DependencyObject *dob;
	DependencyProperty *prop;
} Listener;

//
// Registers @listener as a listener on changes to @child_property of this DO.
//
void
DependencyObject::AddPropertyChangeListener (DependencyObject *listener, DependencyProperty *child_property)
{
	Listener *listen = new Listener ();
	listen->dob = listener;
	listen->prop = child_property;
	listener_list = g_slist_append (listener_list, listen);
}

//
// Unregisters @container as a listener on changes to @child_property of this DO.
//
void
DependencyObject::RemovePropertyChangeListener (DependencyObject *listener, DependencyProperty *child_property)
{
	for (GSList *l = listener_list; l; l = l->next) {
		Listener *listen = (Listener*)l->data;

		if ((listen->dob == listener) && (child_property == NULL || listen->prop == child_property)) {
			listener_list = g_slist_remove_link (listener_list, l);
			delete listen;
		}
	}
}

static void
unregister_depobj_values (gpointer  key,
			  gpointer  value,
			  gpointer  user_data)
{
	DependencyObject *this_obj = (DependencyObject*)user_data;
	//DependencyProperty *prop = (DependencyProperty*)key;
	Value *v = (Value*)value;

	if (v != NULL && v->Is (Type::DEPENDENCY_OBJECT) && v->AsDependencyObject() != NULL) {
		//printf ("unregistering from property %s\n", prop->name);
		DependencyObject *obj = v->AsDependencyObject ();
		obj->RemovePropertyChangeListener (this_obj);
		obj->SetLogicalParent (NULL);
	}
}

#define VALIDATION_ERROR_QUARK validation_error_quark ()

static GQuark
validation_error_quark ()
{
	static GQuark quark = 0;

	if (quark == 0)
		quark = g_quark_from_static_string ("VALIDATION_ERROR_QUARK");
	return quark;
}



void
DependencyObject::RemoveAllListeners ()
{
	g_hash_table_foreach (current_values, unregister_depobj_values, this);
}

static bool listeners_notified;

void
DependencyObject::NotifyListenersOfPropertyChange (PropertyChangedEventArgs *args)
{
	g_return_if_fail (args);

	DependencyObject *logical_parent = GetLogicalParent ();
	bool notified_parent = false;

	listeners_notified = true;

	for (GSList *l = listener_list; l != NULL; l = l->next){
		Listener *listener = (Listener*)l->data;

		listener->dob->OnSubPropertyChanged (listener->prop, this, args);
		if (listener->dob == logical_parent)
			notified_parent = true;
	}

	// attached properties are implicitly listened to by the
	// object's logical parent.  Notify them, but sure not to do
	// it twice.
	if (args->property->is_attached_property && !notified_parent) {
		if (logical_parent /*&& args->property->type == logical_parent->GetObjectType ()*/)
			logical_parent->OnSubPropertyChanged (NULL, this, args);
	}
}

void
DependencyObject::NotifyListenersOfPropertyChange (DependencyProperty *subproperty)
{
	// XXX I really think this method should go away.  we only use it in
	// a couple of places, and it abuses things.

	Value *new_value = subproperty ? GetValue (subproperty) : NULL;

	PropertyChangedEventArgs args (subproperty, NULL, new_value);

	NotifyListenersOfPropertyChange (&args);
}

bool
DependencyObject::IsValueValid (DependencyProperty* property, Value* value, GError** error)
{
	if (property == NULL) {
		g_set_error (error, VALIDATION_ERROR_QUARK, 1001, "NULL property passed to IsValueValid");
		return false;
	}

	if (value != NULL) {
		if (value->Is (Type::EVENTOBJECT) && !value->AsEventObject ()) {
			// if it's a null DependencyObject, it doesn't matter what type it is
			return true;
		}
		
		if (!value->Is (property->value_type)) {
			g_set_error (error, VALIDATION_ERROR_QUARK, 1001,
				     "DependencyObject::SetValue, value cannot be assigned to the "
				     "property %s::%s (property has type '%s', value has type '%s')",
				     GetTypeName (), property->name, Type::Find (property->value_type)->name,
				     Type::Find (value->GetKind ())->name);
			return false;
		}
	} else {
		if (!(Type::IsSubclassOf (property->value_type, Type::DEPENDENCY_OBJECT)) && !property->IsNullable ()) {
			g_set_error (error, VALIDATION_ERROR_QUARK, 1001,
				     "Can not set a non-nullable scalar type to NULL (property: %s)",
				     property->name);
			return false;
		}
	}

	if (DependencyObject::NameProperty == property) {
		NameScope *scope = FindNameScope ();
		if (scope && value && !scope->GetTemporary ()) {
			DependencyObject *o = scope->FindName (value->AsString ());
			if (o && o != this) {
				g_set_error (error, VALIDATION_ERROR_QUARK, 2028,
					     "The name already exists in the tree: %s.",
					     value->AsString ());
				return false;
			}
		}
		// TODO: Name validation
		// This doesn't happen in 1.0 or 2.0b according to my tests, but according to the 2.0 docs
		// names need to start with a '_' or letter.  They can't start with a _.  Also characters
		// should be limited to a-z A-Z 0-9 and _.  Once a newer beta actually enforces this
		// I'll implement the validation method.
	}

	return true;
}

void
DependencyObject::SetValue (DependencyProperty *property, Value *value)
{
	SetValue (property, value, NULL);
}

bool
DependencyObject::SetValue (DependencyProperty* property, Value value, GError** error)
{
	return SetValue (property, &value, error);
}

bool
DependencyObject::SetValue (DependencyProperty* property, Value* value, GError** error)
{
	if (!IsValueValid (property, value, error))
		return false;

	Value *current_value = (Value*)g_hash_table_lookup (current_values, property);
	bool equal = false;

	if (current_value != NULL && value != NULL) {
		equal = !property->always_change && (*current_value == *value);
	} else {
		equal = (current_value == NULL) && (value == NULL);
	}

	if (!equal) {
		DependencyObject *current_as_dep = NULL;
		DependencyObject *new_as_dep = NULL;

		if (current_value && current_value->Is (Type::DEPENDENCY_OBJECT))
			current_as_dep = current_value->AsDependencyObject ();
		if (value && value->Is (Type::DEPENDENCY_OBJECT))
			new_as_dep = value->AsDependencyObject ();

		if (current_as_dep) {
			// unset its logical parent
			current_as_dep->SetLogicalParent (NULL);

			// unregister from the existing value
			current_as_dep->RemovePropertyChangeListener (this, property);
			current_as_dep->SetSurface (NULL);

			// and remove its closure.
			if (current_as_dep->Is (Type::COLLECTION)) {
				Collection *col = (Collection*)current_as_dep;
				col->closure = NULL;
			}
		}

		Value *new_value = value ? new Value (*value) : NULL;

		// store the new value in the hash
		g_hash_table_insert (current_values, property, new_value);

		if (new_as_dep) {
			// set its logical parent
			new_as_dep->SetLogicalParent (this);
			
			// listen for property changes on the new object
			new_as_dep->AddPropertyChangeListener (this, property);
			new_as_dep->SetSurface (GetSurface ());

			// and set its closure to us.
			if (new_as_dep->Is (Type::COLLECTION)) {
				Collection *col = (Collection*)new_as_dep;
				if (col->closure)
					g_warning ("Collection added as property of more than 1 dependency object");
				col->closure = this;
			}

			// merge any temporary namescopes in its
			// subtree.
			MergeTemporaryNameScopes (new_as_dep);
		}
		
		listeners_notified = false;

		PropertyChangedEventArgs args (property, current_value, new_value ? new_value : GetDefaultValue (property));

		OnPropertyChanged (&args);

		if (!listeners_notified)
			g_warning ("setting property %s::%s on object of type %s didn't result in listeners being notified\n",
				   Type::Find(property->type)->GetName (), property->name, GetTypeName ());

		if (current_value)
			delete current_value;
	}

	return true;
}

static NameScope *
create_temp_namescope (DependencyObject *o)
{
	NameScope *ns = new NameScope ();
	ns->SetTemporary (true);
	NameScope::SetNameScope (o, ns);

	return ns;
}

static void
merge_namescope (NameScope *parent_ns, NameScope *child_ns, DependencyObject *owner)
{
	if (!parent_ns)
		parent_ns = create_temp_namescope (owner);
	parent_ns->MergeTemporaryScope (child_ns);

	// remove the child's temporary namescope
	child_ns->ClearValue (NameScope::NameScopeProperty, false);
}

void
DependencyObject::MergeTemporaryNameScopes (DependencyObject *dob)
{
	NameScope *ns = NameScope::GetNameScope (this);
	NameScope *dob_ns = NameScope::GetNameScope (dob);
	
	if (dob_ns && dob_ns->GetTemporary ())
		merge_namescope (ns, dob_ns, this);

	if (dob->Is (Type::COLLECTION)) {
		Collection *c = (Collection*)dob;
		Collection::Node *cn;
		for (cn = (Collection::Node *) c->list->First () ; cn != NULL; cn = (Collection::Node *) cn->next) {
			NameScope *c_ns = NameScope::GetNameScope (cn->obj);
			if (c_ns && c_ns->GetTemporary())
				merge_namescope (ns, c_ns, this);
		}
	}
}

static void
unregister_depobj_names (gpointer  key,
			 gpointer  value,
			 gpointer  user_data)
{
	NameScope *from_ns = (NameScope*)user_data;
	Value *v = (Value*)value;

	if (v != NULL && v->Is (Type::DEPENDENCY_OBJECT) && v->AsDependencyObject() != NULL) {
		DependencyObject *obj = v->AsDependencyObject ();
		obj->UnregisterAllNamesRootedAt (from_ns);
	}
}

static void
register_depobj_names (gpointer  key,
		       gpointer  value,
		       gpointer  user_data)
{
	NameScope *to_ns = (NameScope*)user_data;
	Value *v = (Value*)value;

	if (v != NULL && v->Is (Type::DEPENDENCY_OBJECT) && v->AsDependencyObject() != NULL) {
		DependencyObject *obj = v->AsDependencyObject ();
		obj->RegisterAllNamesRootedAt (to_ns);
	}
}

void
DependencyObject::UnregisterAllNamesRootedAt (NameScope *from_ns)
{
	const char *n = GetName();
	
	if (n)
		from_ns->UnregisterName (n);
	
	g_hash_table_foreach (current_values, unregister_depobj_names, from_ns);
}

void
DependencyObject::RegisterAllNamesRootedAt (NameScope *to_ns)
{
	const char *n = GetName();
	
	if (n)
		to_ns->RegisterName (n, this);

 	g_hash_table_foreach (current_values, register_depobj_names, to_ns);
}

void
DependencyObject::SetValue (DependencyProperty *property, Value value)
{
	SetValue (property, &value);
}

Value *
DependencyObject::GetDefaultValue (DependencyProperty *property)
{
	return property->default_value;
}

Value *
DependencyObject::GetValue (DependencyProperty *property)
{
	void *value = NULL;
	
	if (g_hash_table_lookup_extended (current_values, property, NULL, &value))
		return (Value *) value;
	
	return GetDefaultValue (property);
}

Value *
DependencyObject::GetValueNoDefault (DependencyProperty *property)
{
	return (Value *) g_hash_table_lookup (current_values, property);
}

Value *
DependencyObject::GetValue (const char *name)
{
	DependencyProperty *property;
	property = GetDependencyProperty (name);

	if (property == NULL) {
		g_warning ("This object (of type '%s') doesn't have a property called '%s'\n", GetType ()->GetName (), name);
		return NULL;
	}

	return GetValue (property);
}

void
DependencyObject::ClearValue (DependencyProperty *property, bool notify_listeners)
{
	Value *current_value = GetValueNoDefault (property);

	if (current_value == NULL) {
		/* the property has the default value, nothing to do */
		return;
	}

	// detach from the existing value
	if (current_value->Is (Type::DEPENDENCY_OBJECT)){
		DependencyObject *dob = current_value->AsDependencyObject();

		if (dob != NULL)
			dob->RemovePropertyChangeListener (this, property);

		dob->SetSurface (NULL);
	}

	// and remove it's closure
	if (Type::Find(current_value->GetKind())->IsSubclassOf (Type::COLLECTION)) {
		Collection *col = current_value->AsCollection ();
		if (col)
			col->closure = NULL;
	}

	g_hash_table_remove (current_values, property);

	// we need to make this optional, as doing it for NameScope
	// merging is killing performance (and noone should ever care
	// about that property changing)
	if (notify_listeners) {
		listeners_notified = false;

		PropertyChangedEventArgs args (property, current_value, NULL);

		OnPropertyChanged (&args);

		if (!listeners_notified)
			g_warning ("setting property %s::%s on object of type %s didn't result in listeners being notified\n",
				   Type::Find(property->type)->GetName (), property->name, GetTypeName ());
	}

	delete current_value;
}

void 
DependencyObject::SetValue (const char *name, Value value)
{
	SetValue (name, &value);
}

void
DependencyObject::SetValue (const char *name, Value *value)
{
	DependencyProperty *property;
	property = GetDependencyProperty (name);

	if (property == NULL) {
		g_warning ("This object (of type '%s') doesn't have a property called '%s'\n", GetTypeName (), name);
		return;
	}

	SetValue (property, value);
}

static gboolean
free_value (gpointer key, gpointer value, gpointer data)
{
	Value *v = (Value *) value;
	
	if (!value)
		return true;
	
	// detach from the existing value
	if (v->Is (Type::DEPENDENCY_OBJECT)){
		DependencyObject *dob = v->AsDependencyObject();
		
		if (dob != NULL)
			dob->RemovePropertyChangeListener ((DependencyObject*)data, NULL);
	}
	
	// and remove it's closure
	if (Type::Find(v->GetKind())->IsSubclassOf (Type::COLLECTION)) {
		Collection *col = v->AsCollection ();
		if (col)
			col->closure = NULL;
	}
	
	delete (Value *) value;
	
	return true;
}

DependencyObject::DependencyObject ()
{
	current_values = g_hash_table_new (g_direct_hash, g_direct_equal);
	this->listener_list = NULL;
	this->logical_parent = NULL;
}

static void
dump (gpointer key, gpointer value, gpointer data)
{
	printf ("%s\n", (char*)key);
}

Type::Kind
DependencyObject::GetObjectType ()
{
	g_critical ("%p This class is missing an override of GetObjectType ()", this);
	d(print_stack_trace ());
	
	g_hash_table_foreach (current_values, dump, NULL);
	return Type::DEPENDENCY_OBJECT; 
}

static void
free_listener (gpointer data, gpointer user_data)
{
	Listener* listener = (Listener*) data;
	delete listener;
}

DependencyObject::~DependencyObject ()
{
	if (listener_list != NULL) {
		g_slist_foreach (listener_list, free_listener, NULL);
		g_slist_free (listener_list);
	}

	RemoveAllListeners();
	g_hash_table_foreach_remove (current_values, free_value, this);
	g_hash_table_destroy (current_values);
}

DependencyProperty *
DependencyObject::GetDependencyProperty (const char *name)
{
	return DependencyObject::GetDependencyProperty (GetObjectType (), name);
}

DependencyProperty *
DependencyObject::GetDependencyProperty (Type::Kind type, const char *name)
{
	return GetDependencyProperty (type, name, true);
}

DependencyProperty *
DependencyObject::GetDependencyProperty (Type::Kind type, const char *name, bool inherits)
{
	GHashTable *table;
	DependencyProperty *property = NULL;

	if (properties == NULL)
		return NULL;

	table = (GHashTable*) g_hash_table_lookup (properties, &type);

	if (table) {
		char *key = g_ascii_strdown (name, -1);
		property = (DependencyProperty*) g_hash_table_lookup (table, key);
		g_free (key);
	}

	if (property != NULL)
		return property;

	if (!inherits)
		return NULL;	

	// Look in the parent type
	Type *current_type;
	current_type = Type::Find (type);
	
	if (current_type == NULL)
		return NULL;
	
	if (current_type->GetParent () == Type::INVALID)
		return NULL;

	return GetDependencyProperty (current_type->GetParent (), name);
}

bool
DependencyObject::HasProperty (const char *name, bool inherits)
{
	return GetDependencyProperty (GetObjectType (), name, inherits) != NULL;
}

DependencyObject *
DependencyObject::FindName (const char *name)
{
	NameScope *scope = NameScope::GetNameScope (this);
	DependencyObject *rv = NULL;
	
	if (scope && (rv = scope->FindName (name)))
		return rv;
	
	if (logical_parent)
		return logical_parent->FindName (name);
	
	Surface *surface = GetSurface ();
	if (surface) {
		UIElement *toplevel = surface->GetToplevel ();
		if (toplevel && toplevel != this)
			return toplevel->FindName (name);
	}
	
	return NULL;
}

NameScope*
DependencyObject::FindNameScope ()
{
	NameScope *scope = NameScope::GetNameScope (this);

	if (scope)
		return scope;

	if (logical_parent)
		return logical_parent->FindNameScope ();

	return NULL;
}
		 
DependencyObject *
dependency_object_find_name (DependencyObject *obj, const char *name, Type::Kind *element_kind)
{
	if (obj == NULL)
		return NULL;
	//printf ("Looking up in %p the string %p\n", obj, name);
	//printf ("        String: %s\n", name);
	DependencyObject *ret = obj->FindName (name);

	if (ret == NULL)
		return NULL;

	*element_kind = ret->GetObjectType ();

	return ret;
}

//
//  A helper debugging routine for C#
//
const char *
dependency_object_get_name (DependencyObject *obj)
{
	return obj->GetName ();
}

Type::Kind
dependency_object_get_object_type (DependencyObject *obj)
{
	return obj->GetObjectType ();
}

const char *
dependency_object_get_type_name (DependencyObject *obj)
{
	return obj->GetTypeName ();
}

// Used by routines which need to create DO from code
void
dependency_object_set_name (DependencyObject *obj, const char *name)
{
	obj->SetValue (DependencyObject::NameProperty, Value (name));
}

//
// Use this for values that can be null
//
DependencyProperty *
DependencyObject::Register (Type::Kind type, const char *name, Type::Kind vtype)
{
	g_return_val_if_fail (name != NULL, NULL);

	return RegisterFull (type, name, NULL, vtype, false, false);
}

//
// DependencyObject takes ownership of the Value * for default_value
//
DependencyProperty *
DependencyObject::Register (Type::Kind type, const char *name, Value *default_value)
{
	g_return_val_if_fail (default_value != NULL, NULL);
	g_return_val_if_fail (name != NULL, NULL);

	return RegisterFull (type, name, default_value, default_value->GetKind (), false, false);
}

static void
free_property_hash (gpointer v)
{
	g_hash_table_destroy ((GHashTable*)v);
}

static void
free_property (gpointer v)
{
	delete (DependencyProperty*)v;
}

//
// DependencyObject takes ownership of the Value * for default_value
// This overload can be used to set the type of the property to a different type
// than the default value (the default value can for instance be a SolidColorBrush
// while the property type can be a Brush).
//
DependencyProperty *
DependencyObject::Register (Type::Kind type, const char *name, Value *default_value, Type::Kind vtype)
{
	g_return_val_if_fail (default_value != NULL, NULL);
	g_return_val_if_fail (name != NULL, NULL);

	return RegisterFull (type, name, default_value, vtype, false, false);
}

DependencyProperty *
DependencyObject::RegisterNullable (Type::Kind type, const char *name, Type::Kind vtype)
{
	DependencyProperty *property;
	property = Register (type, name, vtype);
	property->is_nullable = true;
	return property;
}

//
// Register the dependency property that belongs to @type with the name @name
// The default value is @default_value (if provided) and the type that can be
// stored in the dependency property is of type @vtype
//
DependencyProperty *
DependencyObject::RegisterFull (Type::Kind type, const char *name, Value *default_value, Type::Kind vtype, bool attached, bool readonly, bool always_change)
{
	GHashTable *table;

	DependencyProperty *property = new DependencyProperty (type, name, default_value, vtype, attached, readonly, always_change);
	
	/* first add the property to the global 2 level property hash */
	if (properties == NULL)
		properties = g_hash_table_new_full (g_int_hash, g_int_equal,
						    NULL, free_property_hash);

	table = (GHashTable*) g_hash_table_lookup (properties, &property->type);

	if (table == NULL) {
		table = g_hash_table_new_full (g_str_hash, g_str_equal,
					       NULL, free_property);
		g_hash_table_insert (properties, &property->type, table);
	}

	g_hash_table_insert (table, property->hash_key, property);

	return property;
}

void
DependencyObject::Shutdown ()
{
	drain_unrefs ();
	g_hash_table_destroy (DependencyObject::properties);
	DependencyObject::properties = NULL;
}

static void
set_surface (gpointer key, gpointer value, gpointer data)
{
	Surface *s = (Surface *) data;
	Value *v = (Value *) value;
	
	if (v && v->Is (Type::DEPENDENCY_OBJECT)) {
		DependencyObject *dob = v->AsDependencyObject();
		dob->SetSurface (s);
	}
}

void
DependencyObject::SetSurface (Surface *s)
{
	EventObject::SetSurface (s);
	g_hash_table_foreach (current_values, set_surface, s);
}

void
DependencyObject::SetLogicalParent (DependencyObject *logical_parent)
{
#if DEBUG
	// Check for circular families
	DependencyObject *current = logical_parent;
	while (current != NULL) {
		g_assert (current != this); // (assert within #if DEBUG)
		current = current->GetLogicalParent ();
	} 
#endif
	this->logical_parent = logical_parent;
}

DependencyObject *
DependencyObject::GetLogicalParent ()
{
	DependencyObject *res = logical_parent;
	while (res && Type::Find (res->GetObjectType ())->IsSubclassOf (Type::COLLECTION))
		res = res->GetLogicalParent ();
	return res;
}

Value *
dependency_object_get_value (DependencyObject *object, DependencyProperty *prop)
{
	if (object == NULL)
		return NULL;

	return object->GetValue (prop);
}

Value *
dependency_object_get_value_no_default (DependencyObject *object, DependencyProperty *prop)
{
	if (object == NULL)
		return NULL;
	
	return object->GetValueNoDefault (prop);
}

void
dependency_object_set_value (DependencyObject *object, DependencyProperty *prop, Value *val)
{
	if (object == NULL)
		return;

	object->SetValue (prop, val);
}

/*
 *	DependencyProperty
 */
DependencyProperty::DependencyProperty (Type::Kind type, const char *name, Value *default_value, Type::Kind value_type, bool attached, bool readonly, bool always_change)
{
	this->type = type;
	this->hash_key = g_ascii_strdown (name, -1);
	this->name = g_strdup (name);
	this->default_value = default_value;
	this->value_type = value_type;
	this->is_nullable = false;
	this->is_attached_property = attached;
	this->is_readonly = readonly;
	this->storage_hash = NULL; // Create it on first usage request
	this->always_change = always_change;
}

AnimationStorage*
DependencyProperty::GetAnimationStorageFor (DependencyObject *obj)
{
	if (! storage_hash)
		return NULL;

	return (AnimationStorage *) g_hash_table_lookup (storage_hash, obj);
}

AnimationStorage*
DependencyProperty::AttachAnimationStorage (DependencyObject *obj, AnimationStorage *storage)
{
	// Create hash on first access to save some mem
	if (! storage_hash)
		storage_hash = g_hash_table_new (g_direct_hash, g_direct_equal);

	AnimationStorage *attached_storage = (AnimationStorage *) g_hash_table_lookup (storage_hash, obj);
	if (attached_storage)
		attached_storage->DetachTarget ();

	g_hash_table_insert (storage_hash, obj, storage);
	return attached_storage;
}

void
DependencyProperty::DetachAnimationStorage (DependencyObject *obj, AnimationStorage *storage)
{
	if (! storage_hash)
		return;

	if (g_hash_table_lookup (storage_hash, obj) == storage)
		g_hash_table_remove (storage_hash, obj);
}

static void
detach_target_func (DependencyObject *obj, AnimationStorage *storage)
{
	storage->DetachTarget ();
}

DependencyProperty::~DependencyProperty ()
{
	g_free (name);
	if (default_value != NULL)
		delete default_value;

	if (storage_hash) {
		g_hash_table_foreach (storage_hash, (GHFunc) detach_target_func, NULL);
		g_hash_table_destroy (storage_hash);
		storage_hash = NULL;
	}
}

DependencyProperty *dependency_property_lookup (Type::Kind type, char *name)
{
	return DependencyObject::GetDependencyProperty (type, name);
}

char*
dependency_property_get_name (DependencyProperty *property)
{
	return property->name;
}

Type::Kind
dependency_property_get_value_type (DependencyProperty *property)
{
	return property->value_type;
}

bool
dependency_property_is_nullable (DependencyProperty *property)
{
	return property->IsNullable ();
}


//
// Everything inside of a ( ) resolves to a DependencyProperty, if there is a
// '.' after the property, we get the object, and continue resolving from there
// if there is a [n] after the property, we convert the property to a collection
// and grab the nth item.
//
// Dependency properties can be specified as (PropertyName) of the current object
// or they can be specified as (DependencyObject.PropertyName).
//
// Returns NULL on any error
//
DependencyProperty *
resolve_property_path (DependencyObject **o, const char *path)
{
	g_return_val_if_fail (o != NULL, NULL);
	g_return_val_if_fail (path != NULL, NULL);

	int c;
	int len = strlen (path);
	char *typen = NULL;
	char *propn = NULL;
	bool expression_found = false;
	DependencyProperty *res = NULL;
	DependencyObject *lu = *o;
	const char *prop = path;
	
	for (int i = 0; i < len; i++) {
		switch (path [i]) {
		case '(':
		{
			expression_found = true;

			typen = NULL;
			propn = NULL;
			int estart = i + 1;
			for (c = estart; c < len; c++) {
				if (path [c] == '.') {
					// Some Beta versions of Blend had a bug where they would save the TextBlock
					// properties as TextElement instead. Since Silverlight 1.0 works around this
					// bug, we should too. Fixes http://silverlight.timovil.com and
					// http://election.msn.com/podium08.aspx.
					if ((c - estart) == 11 && !g_ascii_strncasecmp (path + estart, "TextElement", 11))
						typen = g_strdup ("TextBlock");
					else
						typen = g_strndup (path + estart, c - estart);
					estart = c + 1;
					continue;
				}
				if (path [c] == ')') {
					propn = g_strndup (path + estart, c - estart);
					break;
				}
			}

			i = c;
			
			Type *t = NULL;
			if (typen) {
				t = Type::Find (typen);
			} else
				t = Type::Find (lu->GetObjectType ());

			if (!t || !propn || (strlen (propn) == 0)) {
				g_free (propn);
				g_free (typen);
				*o = NULL;
				return NULL;
			}
	
			res = DependencyObject::GetDependencyProperty (t->GetKind (), propn);
			if (!res) {
				g_warning ("Can't find '%s' property in '%s'", propn, typen);
				g_free (propn);
				g_free (typen);
				*o = NULL;
				return NULL;
			}

			if (! res->is_attached_property && ! lu->Is (t->GetKind ())) {
				// We try to be gracefull here and do something smart...
				res = DependencyObject::GetDependencyProperty (lu->GetObjectType (), propn);

				if (! res) {
					g_warning ("Got '%s' but expected a type of '%s'!", typen, lu->GetTypeName ());
					g_free (propn);
					g_free (typen);
					*o = NULL;
					return NULL;
				} else {
					g_warning ("Got '%s' but expected a type of '%s' ! "
						   "Did you mean '%s.%s' ? Using that.",
						   typen, lu->GetTypeName (), lu->GetTypeName (), propn);
				}
			}

			g_free (propn);
			g_free (typen);
			break;
		}
		case '.':
			// do not process unless we processed a '(' earlier
			if (!res)
				break;
			lu = lu->GetValue (res)->AsDependencyObject ();
			expression_found = false;
			prop = path + (i + 1);
			// we can ignore this, since we pull the lookup object when we finish a ( ) block
			break;
		case '[':
		{
			int indexer = 0;

			// Need to be a little more loving
			if (path [i + 1] == 0)
				break;

			char *p;

			indexer = strtol (path + i + 1, &p, 10);
			i = p - path;

			if (path [i] != ']'
			    || path [i + 1] != '.')
				break;

			Collection *col = lu->GetValue (res)->AsCollection ();
			List::Node *n = col->list->Index (indexer);
			if (n)
				lu = ((Collection::Node *) n)->obj;
			else {
				g_warning ("%s collection doesn't have element %d!", lu->GetTypeName (), indexer);
				*o = NULL;
				return NULL;
			}

			i += 1;
			break;
		}
		}
	}

	if (!expression_found)
		res = DependencyObject::GetDependencyProperty (lu->GetObjectType (), prop);

	*o = lu;
	return res;
}

DependencyProperty* DependencyObject::NameProperty;

void
DependencyObject::OnPropertyChanged (PropertyChangedEventArgs *args)
{
	if (DependencyObject::NameProperty == args->property) {
		NameScope *scope = FindNameScope ();
		if (scope && args->new_value)
			scope->RegisterName (args->new_value->AsString (), this);
	}

	NotifyListenersOfPropertyChange (args);
}

void
dependencyobject_init(void)
{
	DependencyObject::NameProperty = DependencyObject::Register (Type::DEPENDENCY_OBJECT, "Name", Type::STRING);
}

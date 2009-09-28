/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * eventargs.cpp: specialized code for dealing with mouse/stylus/keyboard event args.
 *
 * Copyright 2007 Novell, Inc. (http://www.novell.com)
 *
 * See the LICENSE file included with the distribution for details.
 *
 */

#include <config.h>

#include "eventargs.h"
#include "uielement.h"
#include "collection.h"
#include "stylus.h"
#include "runtime.h"

EventArgs::EventArgs ()
	: DependencyObject (Type::EVENTARGS)
{
}

EventArgs::EventArgs (Type::Kind kind)
	: DependencyObject (kind)
{
}

EventArgs::~EventArgs ()
{
}

RenderingEventArgs::RenderingEventArgs (TimeSpan renderingTime)
	: EventArgs (Type::RENDERINGEVENTARGS)
{

	this->renderingTime = renderingTime;
}

RenderingEventArgs::~RenderingEventArgs ()
{
}

TimeSpan
RenderingEventArgs::GetRenderingTime ()
{
	return renderingTime;
}



CollectionChangedEventArgs::CollectionChangedEventArgs ()
	: EventArgs (Type::COLLECTIONCHANGEDEVENTARGS)
{
	action = CollectionChangedActionAdd;
	old_item = NULL;
	new_item = NULL;
	index = -1;
}

CollectionChangedEventArgs::CollectionChangedEventArgs (CollectionChangedAction action, Value *new_item, Value *old_item, int index)
	: EventArgs (Type::COLLECTIONCHANGEDEVENTARGS)
{
	this->action = action;
	this->new_item = new_item;
	this->old_item = old_item;
	this->index = index;
}

CollectionChangedEventArgs::~CollectionChangedEventArgs ()
{
}

void
CollectionChangedEventArgs::SetChangedAction (CollectionChangedAction action)
{
	this->action = action;
}
	
CollectionChangedAction
CollectionChangedEventArgs::GetChangedAction ()
{
	return action;
}
	
void
CollectionChangedEventArgs::SetNewItem (Value *item)
{
	new_item = item;
}
	
Value *
CollectionChangedEventArgs::GetNewItem ()
{
	return new_item;
}
	
void
CollectionChangedEventArgs::SetOldItem (Value *item)
{
	old_item = item;
}
	
Value *
CollectionChangedEventArgs::GetOldItem ()
{
	return old_item;
}
	
void
CollectionChangedEventArgs::SetIndex (int index)
{
	this->index = index;
}
	
int
CollectionChangedEventArgs::GetIndex ()
{
	return index;
}

DownloadProgressEventArgs::DownloadProgressEventArgs (double progress)
	: EventArgs (Type::DOWNLOADPROGRESSEVENTARGS)
{
	this->progress = progress;
}

DownloadProgressEventArgs::DownloadProgressEventArgs ()
	: EventArgs (Type::DOWNLOADPROGRESSEVENTARGS)
{
	progress = 0.0;
}

DownloadProgressEventArgs::~DownloadProgressEventArgs ()
{
}

void
DownloadProgressEventArgs::SetProgress (double progress)
{
	this->progress = progress;
}
	
double
DownloadProgressEventArgs::GetProgress ()
{
	return progress;
}

ExceptionRoutedEventArgs::ExceptionRoutedEventArgs ()
	: RoutedEventArgs (Type::EXCEPTIONROUTEDEVENTARGS)
{
}

ExceptionRoutedEventArgs::~ExceptionRoutedEventArgs ()
{
}

RoutedEventArgs::RoutedEventArgs (DependencyObject *source)
	: EventArgs (Type::ROUTEDEVENTARGS)
{
	if (source)
		source->ref ();
	
	this->source = source;
	handled = false;
}

RoutedEventArgs::RoutedEventArgs ()
	: EventArgs (Type::ROUTEDEVENTARGS)
{
	source = NULL;
	handled = false;
}

RoutedEventArgs::RoutedEventArgs (Type::Kind kind)
	: EventArgs (kind)
{
	source = NULL;
	handled = false;
}

RoutedEventArgs::RoutedEventArgs (DependencyObject *source, Type::Kind kind)
{
	if (source)
		source->ref ();
	this->source = source;
	handled = false;
}

RoutedEventArgs::~RoutedEventArgs ()
{
	if (source)
		source->unref ();
}

void
RoutedEventArgs::SetHandled (bool handled)
{
	this->handled = handled;
}
	
bool
RoutedEventArgs::GetHandled ()
{
	return handled;
}

DependencyObject *
RoutedEventArgs::GetSource ()
{
	return source;
}

void
RoutedEventArgs::SetSource (DependencyObject *el)
{
	if (source)
		source->unref();
	source = el;
	if (source)
		source->ref();
}

MouseEventArgs::MouseEventArgs (MoonMouseEvent *event)
	: RoutedEventArgs (Type::MOUSEEVENTARGS)
{
	this->event = (MoonMouseEvent*)event->Clone();
}

MouseEventArgs::MouseEventArgs ()
	: RoutedEventArgs (Type::MOUSEEVENTARGS)
{
	event = NULL;
}

MouseEventArgs::~MouseEventArgs ()
{
	delete event;
}

void
MouseEventArgs::GetPosition (UIElement *relative_to, double *x, double *y)
{
       *x = *y = 0;

       if (event) {
	       Point p = event->GetPosition ();
	       *x = p.x;
	       *y = p.y;
       }

       if (relative_to) {
	       // FIXME this a nasty place to do this we should be able to
	       // reduce the problem for this kind of hit testing.
	       if (relative_to->GetSurface())
		       relative_to->GetSurface()->ProcessDirtyElements ();
	       relative_to->TransformPoint (x, y);
       }
}

StylusInfo*
MouseEventArgs::GetStylusInfo ()
{
	g_assert_not_reached ();
#if 0
	TabletDeviceType type = TabletDeviceTypeMouse;
	bool is_inverted = false;
	GdkDevice *gdk_device;

	switch (event->type) {
	case GDK_MOTION_NOTIFY:
		gdk_device = ((GdkEventMotion*)event)->device;
		break;
	case GDK_BUTTON_PRESS:
	case GDK_BUTTON_RELEASE:
		gdk_device = ((GdkEventButton*)event)->device;
		break;

	default:
	case GDK_ENTER_NOTIFY:
	case GDK_LEAVE_NOTIFY:
		/* GdkEventCrossing doesn't have a device field.  ugh */
		gdk_device = NULL;
		break;
	}

	if (gdk_device) {
		switch (gdk_device->source) {
		case GDK_SOURCE_PEN:
		case GDK_SOURCE_ERASER:
			type = TabletDeviceTypeStylus;
			break;
		case GDK_SOURCE_MOUSE:
		case GDK_SOURCE_CURSOR: /* XXX not sure where to lump this in..  in the stylus block? */
		default:
			type = TabletDeviceTypeMouse;
			break;
		}

		is_inverted = (gdk_device->source == GDK_SOURCE_ERASER);
	}

	StylusInfo *info = new StylusInfo ();

	info->SetValue (StylusInfo::DeviceTypeProperty, Value (type));
	info->SetValue (StylusInfo::IsInvertedProperty, Value (is_inverted));

	return info;
#endif
}

StylusPointCollection*
MouseEventArgs::GetStylusPoints (UIElement *ink_presenter)
{
	g_assert_not_reached ();
#if 0
	StylusPointCollection *points = new StylusPointCollection ();
	double pressure;
	double x, y;
	
	GetPosition (ink_presenter, &x, &y);
	if (!((GdkEventMotion *) event)->device || !gdk_event_get_axis (event, GDK_AXIS_PRESSURE, &pressure))
		pressure = 0.0;
	
	StylusPoint *point = new StylusPoint ();
	point->SetValue (StylusPoint::XProperty, Value(x));
	point->SetValue (StylusPoint::YProperty, Value(y));
	point->SetValue (StylusPoint::PressureFactorProperty, Value(pressure));

	points->Add (point);

	point->unref ();

	return points;
#endif
}

MouseWheelEventArgs::MouseWheelEventArgs (MoonScrollWheelEvent *event)
	: RoutedEventArgs (Type::MOUSEWHEELEVENTARGS)
{
	this->event = (MoonScrollWheelEvent*)event->Clone ();
}

MouseWheelEventArgs::MouseWheelEventArgs ()
	: RoutedEventArgs (Type::MOUSEWHEELEVENTARGS)
{
	event = NULL;
}


MouseWheelEventArgs::~MouseWheelEventArgs ()
{
	delete event;
}

int
MouseWheelEventArgs::GetWheelDelta ()
{
	return event ? event->GetWheelDelta () : 0;
}

KeyEventArgs::KeyEventArgs ()
	: RoutedEventArgs (Type::KEYEVENTARGS)
{
	this->event = NULL;
}

KeyEventArgs::KeyEventArgs (MoonKeyEvent *event)
	: RoutedEventArgs (Type::KEYEVENTARGS)
{
	this->event = (MoonKeyEvent*)event->Clone ();
}

KeyEventArgs::~KeyEventArgs ()
{
	delete event;
}

MoonKeyEvent *
KeyEventArgs::GetEvent ()
{
	return event;
}

int
KeyEventArgs::GetKey ()
{
	return event ? event->GetSilverlightKey() : KeyUNKNOWN;
}

int
KeyEventArgs::GetPlatformKeyCode ()
{
	return event ? event->GetPlatformKeycode() : 0;
}

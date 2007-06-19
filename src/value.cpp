/*
 * value.cpp: Implementation of for Value.
 *
 * Author:
 *   Rolf Bjarne Kvinge (RKvinge@novell.com)
 *
 * Copyright 2007 Novell, Inc. (http://www.novell.com)
 *
 * See the LICENSE file included with the distribution for details.
 * 
 */

#include <config.h>
#include <string.h>
#include <gtk/gtk.h>
#include <cairo.h>
#include <malloc.h>
#include <stdlib.h>
#include "value.h"
#include "runtime.h"
#include "clock.h"
#include "animation.h"


/**
 * Value implementation
 */

Type::Kind
Value::GetKind ()
{
	return k;
}

void
Value::Init ()
{
	memset (&u, 0, sizeof (u));
}

Value::Value()
  : k (Type::INVALID)
{
	Init ();
}

Value::Value (const Value& v)
{
	k = v.k;
	u = v.u;

	/* make a copy of the string instead of just the pointer */
	switch (k) {
	case Type::STRING:
		u.s = g_strdup (v.u.s);
		break;
	case Type::POINT_ARRAY:
		u.point_array->basic.refcount++;
		break;
	case Type::DOUBLE_ARRAY:
		u.double_array->basic.refcount++;
		break;
	case Type::MATRIX:
		memcpy (u.matrix, v.u.matrix, sizeof(Matrix));
		break;
	case Type::COLOR:
		u.color = new Color (*v.u.color);
		break;
	case Type::POINT:
		u.point = new Point (*v.u.point);
		break;
	case Type::RECT:
		u.rect = new Rect (*v.u.rect);
		break;
	case Type::REPEATBEHAVIOR:
		u.repeat = new RepeatBehavior (*v.u.repeat);
		break;
	case Type::DURATION:
		u.duration = new Duration (*v.u.duration);
		break;
	case Type::KEYTIME:
		u.keytime = new KeyTime (*v.u.keytime);
		break;
	default:
		if (k >= Type::DEPENDENCY_OBJECT)
			u.dependency_object->ref ();
		break;
	}
}

Value::Value (Type::Kind k)
{
	Init();
	this->k = k;
}

Value::Value(bool z)
{
	Init ();
	k = Type::BOOL;
	u.i32 = z;
}

Value::Value (double d)
{
	Init ();
	k = Type::DOUBLE;
	u.d = d;
}

Value::Value (guint64 i)
{
	Init ();
	k = Type::UINT64;
	u.ui64 = i;
}

Value::Value (gint64 i)
{
	Init ();
	k = Type::INT64;
	u.i64 = i;
}

Value::Value (gint32 i)
{
	Init ();
	k = Type::INT32;
	u.i32 = i;
}

Value::Value (Color c)
{
	Init ();
	k = Type::COLOR;
	u.color = new Color (c);
}

Value
value_color_from_argb (uint32_t c)
{
	return Value (Color (c));
}

Value::Value (DependencyObject *obj)
{
	Init ();
	if (obj == NULL) {
		k = Type::DEPENDENCY_OBJECT;
	} else {
		g_assert (obj->GetObjectType () >= Type::DEPENDENCY_OBJECT);
		k = obj->GetObjectType ();
		obj->ref ();
	}
	u.dependency_object = obj;
}

Value::Value (Point pt)
{
	Init ();
	k = Type::POINT;
	u.point = new Point (pt);
}

Value::Value (Rect rect)
{
	Init ();
	k = Type::RECT;
	u.rect = new Rect (rect);
}

Value::Value (RepeatBehavior repeat)
{
	Init();
	k = Type::REPEATBEHAVIOR;
	u.repeat = new RepeatBehavior (repeat);
}

Value::Value (Duration duration)
{
	Init();
	k = Type::DURATION;
	u.duration = new Duration (duration);
}

Value::Value (KeyTime keytime)
{
	Init ();
	k = Type::KEYTIME;
	u.keytime = new KeyTime (keytime);
}

Value::Value (const char* s)
{
	Init ();
	k = Type::STRING;
	u.s= g_strdup (s);
}

Value::Value (Point *points, int count)
{
	Init ();
	k = Type::POINT_ARRAY;
	u.point_array = point_array_new (count, points);
}

Value::Value (double *values, int count)
{
	Init ();
	k = Type::DOUBLE_ARRAY;
	u.double_array = double_array_new (count, values);
}

Value::Value (Matrix *matrix)
{
	Init ();
	k = Type::MATRIX;
	u.matrix = (Matrix*) g_malloc (sizeof (Matrix));
	memcpy (u.matrix, matrix, sizeof (Matrix));
}

Value::~Value ()
{
	switch (GetKind ()) {
	case Type::STRING:
		g_free (u.s);
		break;
	case Type::POINT_ARRAY:
		if (u.point_array != NULL &&--u.point_array->basic.refcount == 0)
			g_free (u.point_array);
		break;
	case Type::DOUBLE_ARRAY:
		if (u.double_array != NULL && --u.double_array->basic.refcount == 0)
			g_free (u.double_array);
		break;
	case Type::MATRIX:
		g_free (u.matrix);
		break;
	case Type::COLOR:
		delete u.color;
		break;
	case Type::POINT:
		delete u.point;
		break;
	case Type::RECT:
		delete u.rect;
		break;
	case Type::REPEATBEHAVIOR:
		delete u.repeat;
		break;
	case Type::DURATION:
		delete u.duration;
		break;
	case Type::KEYTIME:
		delete u.keytime;
		break;
	default:
		if (GetKind () >= Type::DEPENDENCY_OBJECT && u.dependency_object)
			u.dependency_object->unref ();
	}
}

bool            Value::AsBool () { checked_get_exact (Type::BOOL, false, (bool)u.i32); }
double          Value::AsDouble () { checked_get_exact (Type::DOUBLE, 0.0, u.d); }
guint64         Value::AsUint64 () { checked_get_exact (Type::UINT64, 0, u.ui64); }
gint64          Value::AsInt64 () { checked_get_exact (Type::INT64, 0, u.i64); }
gint32          Value::AsInt32 () { checked_get_exact (Type::INT32, 0, u.i32); }
Color*          Value::AsColor () { checked_get_exact (Type::COLOR, NULL, u.color); }
Point*          Value::AsPoint () { checked_get_exact (Type::POINT, NULL, u.point); }
Rect*           Value::AsRect  () { checked_get_exact (Type::RECT, NULL, u.rect); }
char*           Value::AsString () { checked_get_exact (Type::STRING, NULL, u.s); }
PointArray*     Value::AsPointArray () { checked_get_exact (Type::POINT_ARRAY, NULL, u.point_array); }
DoubleArray*    Value::AsDoubleArray () { checked_get_exact (Type::DOUBLE_ARRAY, NULL, u.double_array); }

RepeatBehavior* Value::AsRepeatBehavior () { checked_get_exact (Type::REPEATBEHAVIOR, NULL, u.repeat); }
Duration*       Value::AsDuration () { checked_get_exact (Type::DURATION, NULL, u.duration); }
KeyTime*        Value::AsKeyTime () { checked_get_exact (Type::KEYTIME, NULL, u.keytime); }
Matrix*         Value::AsMatrix () { checked_get_exact (Type::MATRIX, NULL, u.matrix); }

/* nullable primitives (all but bool) */
double*         Value::AsNullableDouble () { checked_get_exact (Type::DOUBLE, NULL, &u.d); }
guint64*        Value::AsNullableUint64 () { checked_get_exact (Type::UINT64, NULL, &u.ui64); }
gint64*         Value::AsNullableInt64 () { checked_get_exact (Type::INT64, NULL, &u.i64); }
gint32*         Value::AsNullableInt32 () { checked_get_exact (Type::INT32, NULL, &u.i32); }

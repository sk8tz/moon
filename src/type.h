/*
 * Automatically generated from type.h.in, do not edit this file directly
 * To regenerate execute typegen.sh
*/
/*
 * type.h: Generated code for the type system.
 *
 * Author:
 *   Rolf Bjarne Kvinge (RKvinge@novell.com)
 *
 * Copyright 2007 Novell, Inc. (http://www.novell.com)
 *
 * See the LICENSE file included with the distribution for details.
 * 
 */
#ifndef __TYPE_H__
#define __TYPE_H__

#include <glib.h>

class DependencyObject;

typedef gint64 TimeSpan;
typedef DependencyObject *create_inst_func (void);


class Type {
public:
	enum Kind {
	// START_MANAGED_MAPPING
		INVALID,
		BOOL,
		DOUBLE,
		UINT64,
		INT32,
		STRING,
		COLOR,
		POINT,
		RECT,
		REPEATBEHAVIOR,
		DURATION,
		INT64,
		TIMESPAN,
		DOUBLE_ARRAY,
		POINT_ARRAY,
		KEYTIME,
		MATRIX,
		NPOBJ,
		EVENTOBJECT,
		SURFACE,
		TIMESOURCE,
		SYSTEMTIMESOURCE,
		MANUALTIMESOURCE,
		TIMEMANAGER,

		DEPENDENCY_OBJECT,
		ANIMATION,
		ANIMATIONCLOCK,
		ARCSEGMENT,
		BEGINSTORYBOARD,
		BEZIERSEGMENT,
		BRUSH,
		CANVAS,
		CLOCK,
		CLOCKGROUP,
		COLLECTION,
		COLORANIMATION,
		COLORANIMATIONUSINGKEYFRAMES,
		COLORKEYFRAME,
		COLORKEYFRAME_COLLECTION,
		CONTROL,
		DISCRETECOLORKEYFRAME,
		DISCRETEDOUBLEKEYFRAME,
		DISCRETEPOINTKEYFRAME,
		DOUBLEANIMATION,
		DOUBLEANIMATIONUSINGKEYFRAMES,
		DOUBLEKEYFRAME,
		DOUBLEKEYFRAME_COLLECTION,
		DOWNLOADER,
		DRAWINGATTRIBUTES,
		ELLIPSE,
		ELLIPSEGEOMETRY,
		EVENTTRIGGER,
		FRAMEWORKELEMENT,
		GEOMETRY,
		GEOMETRY_COLLECTION,
		GEOMETRYGROUP,
		GLYPHS,
		GRADIENTBRUSH,
		GRADIENTSTOP,
		GRADIENTSTOP_COLLECTION,
		IMAGE,
		IMAGEBRUSH,
		INKPRESENTER,
		INLINE,
		INLINES,
		KEYFRAME,
		KEYFRAME_COLLECTION,
		KEYSPLINE,
		LINE,
		LINEARCOLORKEYFRAME,
		LINEARDOUBLEKEYFRAME,
		LINEARGRADIENTBRUSH,
		LINEARPOINTKEYFRAME,
		LINEBREAK,
		LINEGEOMETRY,
		LINESEGMENT,
		MATRIXTRANSFORM,
		MEDIAATTRIBUTE,
		MEDIAATTRIBUTE_COLLECTION,
		MEDIABASE,
		MEDIAELEMENT,
		NAMESCOPE,
		PANEL,
		PARALLELTIMELINE,
		PATH,
		PATHFIGURE,
		PATHFIGURE_COLLECTION,
		PATHGEOMETRY,
		PATHSEGMENT,
		PATHSEGMENT_COLLECTION,
		POINTANIMATION,
		POINTANIMATIONUSINGKEYFRAMES,
		POINTKEYFRAME,
		POINTKEYFRAME_COLLECTION,
		POLYBEZIERSEGMENT,
		POLYGON,
		POLYLINE,
		POLYLINESEGMENT,
		POLYQUADRATICBEZIERSEGMENT,
		QUADRATICBEZIERSEGMENT,
		RADIALGRADIENTBRUSH,
		RECTANGLE,
		RECTANGLEGEOMETRY,
		RESOURCE_DICTIONARY,
		ROTATETRANSFORM,
		RUN,
		SCALETRANSFORM,
		SHAPE,
		SKEWTRANSFORM,
		SOLIDCOLORBRUSH,
		SPLINECOLORKEYFRAME,
		SPLINEDOUBLEKEYFRAME,
		SPLINEPOINTKEYFRAME,
		STORYBOARD,
		STROKE,
		STROKE_COLLECTION,
		STYLUSINFO,
		STYLUSPOINT,
		STYLUSPOINT_COLLECTION,
		TEXTBLOCK,
		TILEBRUSH,
		TIMELINE,
		TIMELINE_COLLECTION,
		TIMELINEGROUP,
		TIMELINEMARKER,
		TIMELINEMARKER_COLLECTION,
		TRANSFORM,
		TRANSFORM_COLLECTION,
		TRANSFORMGROUP,
		TRANSLATETRANSFORM,
		TRIGGERACTION,
		TRIGGERACTION_COLLECTION,
		TRIGGER_COLLECTION,
		UIELEMENT,
		VIDEOBRUSH,
		VISUAL,
		VISUALBRUSH,
		VISUAL_COLLECTION,

	
		LASTTYPE,
	// END_MANAGED_MAPPING
		};

	static Type* RegisterType (const char *name, Type::Kind type, Type::Kind parent, create_inst_func *create_inst, const char *content_property, bool value_type);
	static Type* RegisterType (const char *name, Type::Kind type, Type::Kind parent, create_inst_func *create_inst, const char *content_property);
	static Type* RegisterType (const char *name, Type::Kind type, Type::Kind parent);
	static Type* RegisterType (const char *name, Type::Kind type, bool value_type);

	static Type* Find (const char *name);
	static Type* Find (Type::Kind type);
	
	bool IsSubclassOf (Type::Kind super);	
	Type::Kind parent;
	Type::Kind type;
	char *name;
	bool value_type;

	void RegisterEvent (const char *event_name);
	int LookupEvent (const char *event_name);
	int GetEventCount ();
	int GetEventBase ();

	DependencyObject *CreateInstance ();
	const char *GetContentPropertyName ();

	static void Shutdown ();
private:
	Type (const char *name, Type::Kind type, Type::Kind parent);
	~Type ();
	static Type** types;
	static GHashTable *types_by_name;
	static void free_type (gpointer v);
	GHashTable *event_name_hash;
	int local_event_base;
	int local_event_count;
	int type_event_count; /* local_event_base + local_event_count */
	create_inst_func *create_inst;
	const char *content_property;
};

G_BEGIN_DECLS

bool type_get_value_type (Type::Kind type);
DependencyObject *type_create_instance (Type *type);
DependencyObject *type_create_instance_from_kind (Type::Kind kind);

void types_init (void);

G_END_DECLS

#endif


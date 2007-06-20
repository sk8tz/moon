/*
 * Automatically generated from value.h.in, do not edit this file directly
 * To regenerate execute typegen.sh
*/
#ifndef MOON_VALUE_H
#define MOON_VALUE_H

#include "type.h"

/* ugly file containing the catchall value type we use.
*/

struct Color;
struct DoubleArray;
struct Duration;
struct KeyTime;
struct Point;
struct PointArray;
struct Rect;
struct RepeatBehavior;
struct Value;

typedef cairo_matrix_t Matrix;

class Type;
class DependencyProperty;
class Surface;

class DependencyObject;
class Animation;
class AnimationClock;
class ArcSegment;
class BeginStoryboard;
class BezierSegment;
class Brush;
class Canvas;
class Clock;
class ClockGroup;
class Collection;
class ColorAnimation;
class ColorAnimationUsingKeyFrames;
class ColorKeyFrame;
class Control;
class DiscreteColorKeyFrame;
class DiscreteDoubleKeyFrame;
class DiscretePointKeyFrame;
class DoubleAnimation;
class DoubleAnimationUsingKeyFrames;
class DoubleKeyFrame;
class Downloader;
class Ellipse;
class EllipseGeometry;
class EventTrigger;
class FrameworkElement;
class Geometry;
class GeometryCollection;
class GeometryGroup;
class Glyphs;
class GradientBrush;
class GradientStop;
class GradientStopCollection;
class Image;
class ImageBrush;
class Inline;
class Inlines;
class KeyFrame;
class KeyFrameCollection;
class KeySpline;
class Line;
class LinearColorKeyFrame;
class LinearDoubleKeyFrame;
class LinearGradientBrush;
class LinearPointKeyFrame;
class LineBreak;
class LineGeometry;
class LineSegment;
class MatrixTransform;
class MediaAttribute;
class MediaAttributeCollection;
class MediaBase;
class MediaElement;
class NameScope;
class Panel;
class ParallelTimeline;
class Path;
class PathFigure;
class PathFigureCollection;
class PathGeometry;
class PathSegment;
class PathSegmentCollection;
class PointAnimation;
class PointAnimationUsingKeyFrames;
class PointKeyFrame;
class PolyBezierSegment;
class Polygon;
class Polyline;
class PolyLineSegment;
class PolyQuadraticBezierSegment;
class QuadraticBezierSegment;
class RadialGradientBrush;
class Rectangle;
class RectangleGeometry;
class ResourceCollection;
class RotateTransform;
class Run;
class ScaleTransform;
class Shape;
class SkewTransform;
class SolidColorBrush;
class SplineDoubleKeyFrame;
class Storyboard;
class StrokeCollection;
class StylusPointCollection;
class TextBlock;
class TileBrush;
class Timeline;
class TimelineCollection;
class TimelineGroup;
class TimelineMarker;
class TimelineMarkerCollection;
class Transform;
class TransformCollection;
class TransformGroup;
class TranslateTransform;
class TriggerAction;
class TriggerActionCollection;
class TriggerCollection;
class UIElement;
class VideoBrush;
class Visual;
class VisualCollection;


#define checked_get_exact(kind, errval, mem)  g_return_val_if_fail (k == (kind), errval); return mem;
#define checked_get_subclass(kind, castas)  if (u.dependency_object == NULL) return NULL; g_return_val_if_fail (Type::Find((kind))->IsSubclassOf(k) || Type::Find(k)->IsSubclassOf((kind)), NULL); return (castas*)u.dependency_object;

struct Value {
public:
	void Init ();

	Value ();
	Value (const Value& v);
	Value (Type::Kind k);
	Value (const Value& v, Type::Kind as);
	Value (bool z);
	Value (double d);
	Value (guint64 i);
	Value (gint64 i);
	Value (gint32 i);
	Value (Color c);
	Value (DependencyObject *obj);
	Value (Point pt);
	Value (Rect rect);
	Value (RepeatBehavior repeat);
	Value (Duration duration);
	Value (KeyTime keytime);
	Value (const char* s);
	Value (Point *points, int count);
	Value (double *values, int count);
	Value (Matrix* matrix);
	
	~Value ();

	bool operator!= (const Value &v) const
	{
		return !(*this == v);
	}

	bool operator== (const Value &v) const
	{
		if (k != v.k)
			return false;

		if (k == Type::STRING) {
			return !strcmp (u.s, v.u.s);
		}
		else {
			return !memcmp (&u, &v.u, sizeof (u));
		}

		return true;
	}


	bool		AsBool ()	{ checked_get_exact (Type::BOOL, false, (bool)u.i32); }
	double 		AsDouble ()	{ checked_get_exact (Type::DOUBLE, 0.0, u.d); }
	guint64		AsUint64 ()	{ checked_get_exact (Type::UINT64, 0, u.ui64); }
	gint64		AsInt64 ()	{ checked_get_exact (Type::INT64, 0, u.i64); }
	TimeSpan	AsTimeSpan ()	{ checked_get_exact (Type::TIMESPAN, 0, (TimeSpan)u.i64); }
	gint32		AsInt32 ()	{ checked_get_exact (Type::INT32, 0, u.i32); }
	Color*		AsColor ()	{ checked_get_exact (Type::COLOR, NULL, u.color); }
	Point*		AsPoint ()	{ checked_get_exact (Type::POINT, NULL, u.point); }
	Rect*		AsRect ()	{ checked_get_exact (Type::RECT, NULL, u.rect); }
	char*		AsString ()	{ checked_get_exact (Type::STRING, NULL, u.s); }
	PointArray*	AsPointArray ()	{ checked_get_exact (Type::POINT_ARRAY, NULL, u.point_array); }
	DoubleArray*	AsDoubleArray ()	{ checked_get_exact (Type::DOUBLE_ARRAY, NULL, u.double_array); }
	
	RepeatBehavior*	AsRepeatBehavior ()	{ checked_get_exact (Type::REPEATBEHAVIOR, NULL, u.repeat); }
	Duration*	AsDuration ()	{ checked_get_exact (Type::DURATION, NULL, u.duration); }
	KeyTime*	AsKeyTime ()	{ checked_get_exact (Type::KEYTIME, NULL, u.keytime); }
	Matrix*		AsMatrix ()	{ checked_get_exact (Type::MATRIX, NULL, u.matrix); }

	/* nullable primitives (all but bool) */
	double*		AsNullableDouble ()	{ checked_get_exact (Type::DOUBLE, NULL, &u.d); }
	guint64*	AsNullableUint64 ()	{ checked_get_exact (Type::UINT64, NULL, &u.ui64); }
	gint64*		AsNullableInt64 ()	{ checked_get_exact (Type::INT64, NULL, &u.i64); }
	gint32*		AsNullableInt32 ()	{ checked_get_exact (Type::INT32, NULL, &u.i32); }

	DependencyObject*              AsDependencyObject () { checked_get_subclass (Type::DEPENDENCY_OBJECT, DependencyObject) }
	Animation*                     AsAnimation () { checked_get_subclass (Type::ANIMATION, Animation) }
	AnimationClock*                AsAnimationClock () { checked_get_subclass (Type::ANIMATIONCLOCK, AnimationClock) }
	ArcSegment*                    AsArcSegment () { checked_get_subclass (Type::ARCSEGMENT, ArcSegment) }
	BeginStoryboard*               AsBeginStoryboard () { checked_get_subclass (Type::BEGINSTORYBOARD, BeginStoryboard) }
	BezierSegment*                 AsBezierSegment () { checked_get_subclass (Type::BEZIERSEGMENT, BezierSegment) }
	Brush*                         AsBrush () { checked_get_subclass (Type::BRUSH, Brush) }
	Canvas*                        AsCanvas () { checked_get_subclass (Type::CANVAS, Canvas) }
	Clock*                         AsClock () { checked_get_subclass (Type::CLOCK, Clock) }
	ClockGroup*                    AsClockGroup () { checked_get_subclass (Type::CLOCKGROUP, ClockGroup) }
	Collection*                    AsCollection () { checked_get_subclass (Type::COLLECTION, Collection) }
	ColorAnimation*                AsColorAnimation () { checked_get_subclass (Type::COLORANIMATION, ColorAnimation) }
	ColorAnimationUsingKeyFrames*  AsColorAnimationUsingKeyFrames () { checked_get_subclass (Type::COLORANIMATIONUSINGKEYFRAMES, ColorAnimationUsingKeyFrames) }
	ColorKeyFrame*                 AsColorKeyFrame () { checked_get_subclass (Type::COLORKEYFRAME, ColorKeyFrame) }
	Control*                       AsControl () { checked_get_subclass (Type::CONTROL, Control) }
	DiscreteColorKeyFrame*         AsDiscreteColorKeyFrame () { checked_get_subclass (Type::DISCRETECOLORKEYFRAME, DiscreteColorKeyFrame) }
	DiscreteDoubleKeyFrame*        AsDiscreteDoubleKeyFrame () { checked_get_subclass (Type::DISCRETEDOUBLEKEYFRAME, DiscreteDoubleKeyFrame) }
	DiscretePointKeyFrame*         AsDiscretePointKeyFrame () { checked_get_subclass (Type::DISCRETEPOINTKEYFRAME, DiscretePointKeyFrame) }
	DoubleAnimation*               AsDoubleAnimation () { checked_get_subclass (Type::DOUBLEANIMATION, DoubleAnimation) }
	DoubleAnimationUsingKeyFrames* AsDoubleAnimationUsingKeyFrames () { checked_get_subclass (Type::DOUBLEANIMATIONUSINGKEYFRAMES, DoubleAnimationUsingKeyFrames) }
	DoubleKeyFrame*                AsDoubleKeyFrame () { checked_get_subclass (Type::DOUBLEKEYFRAME, DoubleKeyFrame) }
	Downloader*                    AsDownloader () { checked_get_subclass (Type::DOWNLOADER, Downloader) }
	Ellipse*                       AsEllipse () { checked_get_subclass (Type::ELLIPSE, Ellipse) }
	EllipseGeometry*               AsEllipseGeometry () { checked_get_subclass (Type::ELLIPSEGEOMETRY, EllipseGeometry) }
	EventTrigger*                  AsEventTrigger () { checked_get_subclass (Type::EVENTTRIGGER, EventTrigger) }
	FrameworkElement*              AsFrameworkElement () { checked_get_subclass (Type::FRAMEWORKELEMENT, FrameworkElement) }
	Geometry*                      AsGeometry () { checked_get_subclass (Type::GEOMETRY, Geometry) }
	GeometryCollection*            AsGeometryCollection () { checked_get_subclass (Type::GEOMETRY_COLLECTION, GeometryCollection) }
	GeometryGroup*                 AsGeometryGroup () { checked_get_subclass (Type::GEOMETRYGROUP, GeometryGroup) }
	Glyphs*                        AsGlyphs () { checked_get_subclass (Type::GLYPHS, Glyphs) }
	GradientBrush*                 AsGradientBrush () { checked_get_subclass (Type::GRADIENTBRUSH, GradientBrush) }
	GradientStop*                  AsGradientStop () { checked_get_subclass (Type::GRADIENTSTOP, GradientStop) }
	GradientStopCollection*        AsGradientStopCollection () { checked_get_subclass (Type::GRADIENTSTOP_COLLECTION, GradientStopCollection) }
	Image*                         AsImage () { checked_get_subclass (Type::IMAGE, Image) }
	ImageBrush*                    AsImageBrush () { checked_get_subclass (Type::IMAGEBRUSH, ImageBrush) }
	Inline*                        AsInline () { checked_get_subclass (Type::INLINE, Inline) }
	Inlines*                       AsInlines () { checked_get_subclass (Type::INLINES, Inlines) }
	KeyFrame*                      AsKeyFrame () { checked_get_subclass (Type::KEYFRAME, KeyFrame) }
	KeyFrameCollection*            AsKeyFrameCollection () { checked_get_subclass (Type::KEYFRAME_COLLECTION, KeyFrameCollection) }
	KeySpline*                     AsKeySpline () { checked_get_subclass (Type::KEYSPLINE, KeySpline) }
	Line*                          AsLine () { checked_get_subclass (Type::LINE, Line) }
	LinearColorKeyFrame*           AsLinearColorKeyFrame () { checked_get_subclass (Type::LINEARCOLORKEYFRAME, LinearColorKeyFrame) }
	LinearDoubleKeyFrame*          AsLinearDoubleKeyFrame () { checked_get_subclass (Type::LINEARDOUBLEKEYFRAME, LinearDoubleKeyFrame) }
	LinearGradientBrush*           AsLinearGradientBrush () { checked_get_subclass (Type::LINEARGRADIENTBRUSH, LinearGradientBrush) }
	LinearPointKeyFrame*           AsLinearPointKeyFrame () { checked_get_subclass (Type::LINEARPOINTKEYFRAME, LinearPointKeyFrame) }
	LineBreak*                     AsLineBreak () { checked_get_subclass (Type::LINEBREAK, LineBreak) }
	LineGeometry*                  AsLineGeometry () { checked_get_subclass (Type::LINEGEOMETRY, LineGeometry) }
	LineSegment*                   AsLineSegment () { checked_get_subclass (Type::LINESEGMENT, LineSegment) }
	MatrixTransform*               AsMatrixTransform () { checked_get_subclass (Type::MATRIXTRANSFORM, MatrixTransform) }
	MediaAttribute*                AsMediaAttribute () { checked_get_subclass (Type::MEDIAATTRIBUTE, MediaAttribute) }
	MediaAttributeCollection*      AsMediaAttributeCollection () { checked_get_subclass (Type::MEDIAATTRIBUTE_COLLECTION, MediaAttributeCollection) }
	MediaBase*                     AsMediaBase () { checked_get_subclass (Type::MEDIABASE, MediaBase) }
	MediaElement*                  AsMediaElement () { checked_get_subclass (Type::MEDIAELEMENT, MediaElement) }
	NameScope*                     AsNameScope () { checked_get_subclass (Type::NAMESCOPE, NameScope) }
	Panel*                         AsPanel () { checked_get_subclass (Type::PANEL, Panel) }
	ParallelTimeline*              AsParallelTimeline () { checked_get_subclass (Type::PARALLELTIMELINE, ParallelTimeline) }
	Path*                          AsPath () { checked_get_subclass (Type::PATH, Path) }
	PathFigure*                    AsPathFigure () { checked_get_subclass (Type::PATHFIGURE, PathFigure) }
	PathFigureCollection*          AsPathFigureCollection () { checked_get_subclass (Type::PATHFIGURE_COLLECTION, PathFigureCollection) }
	PathGeometry*                  AsPathGeometry () { checked_get_subclass (Type::PATHGEOMETRY, PathGeometry) }
	PathSegment*                   AsPathSegment () { checked_get_subclass (Type::PATHSEGMENT, PathSegment) }
	PathSegmentCollection*         AsPathSegmentCollection () { checked_get_subclass (Type::PATHSEGMENT_COLLECTION, PathSegmentCollection) }
	PointAnimation*                AsPointAnimation () { checked_get_subclass (Type::POINTANIMATION, PointAnimation) }
	PointAnimationUsingKeyFrames*  AsPointAnimationUsingKeyFrames () { checked_get_subclass (Type::POINTANIMATIONUSINGKEYFRAMES, PointAnimationUsingKeyFrames) }
	PointKeyFrame*                 AsPointKeyFrame () { checked_get_subclass (Type::POINTKEYFRAME, PointKeyFrame) }
	PolyBezierSegment*             AsPolyBezierSegment () { checked_get_subclass (Type::POLYBEZIERSEGMENT, PolyBezierSegment) }
	Polygon*                       AsPolygon () { checked_get_subclass (Type::POLYGON, Polygon) }
	Polyline*                      AsPolyline () { checked_get_subclass (Type::POLYLINE, Polyline) }
	PolyLineSegment*               AsPolyLineSegment () { checked_get_subclass (Type::POLYLINESEGMENT, PolyLineSegment) }
	PolyQuadraticBezierSegment*    AsPolyQuadraticBezierSegment () { checked_get_subclass (Type::POLYQUADRATICBEZIERSEGMENT, PolyQuadraticBezierSegment) }
	QuadraticBezierSegment*        AsQuadraticBezierSegment () { checked_get_subclass (Type::QUADRATICBEZIERSEGMENT, QuadraticBezierSegment) }
	RadialGradientBrush*           AsRadialGradientBrush () { checked_get_subclass (Type::RADIALGRADIENTBRUSH, RadialGradientBrush) }
	Rectangle*                     AsRectangle () { checked_get_subclass (Type::RECTANGLE, Rectangle) }
	RectangleGeometry*             AsRectangleGeometry () { checked_get_subclass (Type::RECTANGLEGEOMETRY, RectangleGeometry) }
	ResourceCollection*            AsResourceCollection () { checked_get_subclass (Type::RESOURCE_COLLECTION, ResourceCollection) }
	RotateTransform*               AsRotateTransform () { checked_get_subclass (Type::ROTATETRANSFORM, RotateTransform) }
	Run*                           AsRun () { checked_get_subclass (Type::RUN, Run) }
	ScaleTransform*                AsScaleTransform () { checked_get_subclass (Type::SCALETRANSFORM, ScaleTransform) }
	Shape*                         AsShape () { checked_get_subclass (Type::SHAPE, Shape) }
	SkewTransform*                 AsSkewTransform () { checked_get_subclass (Type::SKEWTRANSFORM, SkewTransform) }
	SolidColorBrush*               AsSolidColorBrush () { checked_get_subclass (Type::SOLIDCOLORBRUSH, SolidColorBrush) }
	SplineDoubleKeyFrame*          AsSplineDoubleKeyFrame () { checked_get_subclass (Type::SPLINEDOUBLEKEYFRAME, SplineDoubleKeyFrame) }
	Storyboard*                    AsStoryboard () { checked_get_subclass (Type::STORYBOARD, Storyboard) }
	StrokeCollection*              AsStrokeCollection () { checked_get_subclass (Type::STROKE_COLLECTION, StrokeCollection) }
	StylusPointCollection*         AsStylusPointCollection () { checked_get_subclass (Type::STYLUSPOINT_COLLECTION, StylusPointCollection) }
	TextBlock*                     AsTextBlock () { checked_get_subclass (Type::TEXTBLOCK, TextBlock) }
	TileBrush*                     AsTileBrush () { checked_get_subclass (Type::TILEBRUSH, TileBrush) }
	Timeline*                      AsTimeline () { checked_get_subclass (Type::TIMELINE, Timeline) }
	TimelineCollection*            AsTimelineCollection () { checked_get_subclass (Type::TIMELINE_COLLECTION, TimelineCollection) }
	TimelineGroup*                 AsTimelineGroup () { checked_get_subclass (Type::TIMELINEGROUP, TimelineGroup) }
	TimelineMarker*                AsTimelineMarker () { checked_get_subclass (Type::TIMELINEMARKER, TimelineMarker) }
	TimelineMarkerCollection*      AsTimelineMarkerCollection () { checked_get_subclass (Type::TIMELINEMARKER_COLLECTION, TimelineMarkerCollection) }
	Transform*                     AsTransform () { checked_get_subclass (Type::TRANSFORM, Transform) }
	TransformCollection*           AsTransformCollection () { checked_get_subclass (Type::TRANSFORM_COLLECTION, TransformCollection) }
	TransformGroup*                AsTransformGroup () { checked_get_subclass (Type::TRANSFORMGROUP, TransformGroup) }
	TranslateTransform*            AsTranslateTransform () { checked_get_subclass (Type::TRANSLATETRANSFORM, TranslateTransform) }
	TriggerAction*                 AsTriggerAction () { checked_get_subclass (Type::TRIGGERACTION, TriggerAction) }
	TriggerActionCollection*       AsTriggerActionCollection () { checked_get_subclass (Type::TRIGGERACTION_COLLECTION, TriggerActionCollection) }
	TriggerCollection*             AsTriggerCollection () { checked_get_subclass (Type::TRIGGER_COLLECTION, TriggerCollection) }
	UIElement*                     AsUIElement () { checked_get_subclass (Type::UIELEMENT, UIElement) }
	VideoBrush*                    AsVideoBrush () { checked_get_subclass (Type::VIDEOBRUSH, VideoBrush) }
	Visual*                        AsVisual () { checked_get_subclass (Type::VISUAL, Visual) }
	VisualCollection*              AsVisualCollection () { checked_get_subclass (Type::VISUAL_COLLECTION, VisualCollection) }

  
  	Type::Kind GetKind ();
  
  private:
	Type::Kind k;
	union {
		double d;
		guint64 ui64;
		gint64 i64;
		gint32 i32;
		char *s;
		DependencyObject *dependency_object;
		Color *color;
		Point *point;
		Rect *rect;
		RepeatBehavior *repeat;
		Duration *duration;
		KeyTime *keytime;
		PointArray *point_array;
		DoubleArray *double_array;
		Matrix* matrix;
	} u;


	// You don't want to be using this ctor.  it's here to help
	// c++ recognize bad unspecified pointer args to Value ctors
	// (it normally converts them to bool, which we handle, so you
	// never see the error of your ways).  So do the world a
	// favor, and don't expose this ctor. :)
	Value (void* v) { }
};


#endif /* MOON_VALUE_H */

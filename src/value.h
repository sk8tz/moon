/*
 * Automatically generated, do not edit this file directly
 */

/*
 * value.h: Generated code for the type system.
 *
 * Contact:
 *   Moonlight List (moonlight-list@lists.ximian.com)
 *
 * Copyright 2007 Novell, Inc. (http://www.novell.com)
 *
 * See the LICENSE file included with the distribution for details.
 * 
 */

#ifndef MOON_VALUE_H
#define MOON_VALUE_H

#include <glib.h>
#include <string.h>
#include <stdint.h>
#include <cairo.h>

#include "type.h"

/* 
 * ugly file containing the catchall value type we use.
 */

struct Color;
struct Duration;
struct KeyTime;
struct Point;
struct Rect;
struct Size;
struct RepeatBehavior;
struct GridLength;
struct Thickness;
struct CornerRadius;

struct Value;

class Type;
class DependencyProperty;
class Surface;
class AnimationStorage;

class Animation;
class AnimationClock;
class Application;
class ArcSegment;
class AssemblyPart;
class AssemblyPartCollection;
class BeginStoryboard;
class BezierSegment;
class BindingExpression;
class BindingExpressionBase;
class BitmapImage;
class Border;
class Brush;
class Canvas;
class Clock;
class ClockGroup;
class Collection;
class ColorAnimation;
class ColorAnimationUsingKeyFrames;
class ColorKeyFrame;
class ColorKeyFrameCollection;
class ColumnDefinition;
class ColumnDefinitionCollection;
class ContentControl;
class Control;
class ControlTemplate;
class DeepZoomImageTileSource;
class DependencyObject;
class DependencyObjectCollection;
class Deployment;
class DiscreteColorKeyFrame;
class DiscreteDoubleKeyFrame;
class DiscreteObjectKeyFrame;
class DiscretePointKeyFrame;
class DoubleCollection;
class DoubleAnimation;
class DoubleAnimationUsingKeyFrames;
class DoubleKeyFrame;
class DoubleKeyFrameCollection;
class Downloader;
class DrawingAttributes;
class Ellipse;
class EllipseGeometry;
class ErrorEventArgs;
class EventArgs;
class EventObject;
class EventTrigger;
class Expression;
class FrameworkElement;
class FrameworkTemplate;
class GeneralTransform;
class Geometry;
class GeometryCollection;
class GeometryGroup;
class Glyphs;
class GradientBrush;
class GradientStop;
class GradientStopCollection;
class Grid;
class Image;
class ImageBrush;
class ImageErrorEventArgs;
class InkPresenter;
class Inline;
class InlineCollection;
class KeyEventArgs;
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
class ManualTimeSource;
class MarkerReachedEventArgs;
class Matrix;
class MatrixTransform;
class MediaAttribute;
class MediaAttributeCollection;
class MediaBase;
class MediaElement;
class MediaErrorEventArgs;
class MouseEventArgs;
class MultiScaleImage;
class MultiScaleSubImage;
class MultiScaleTileSource;
class NameScope;
class ObjectAnimationUsingKeyFrames;
class ObjectKeyFrame;
class ObjectKeyFrameCollection;
class Panel;
class ParallelTimeline;
class ParserErrorEventArgs;
class Path;
class PathFigure;
class PathFigureCollection;
class PathGeometry;
class PathSegment;
class PathSegmentCollection;
class PointCollection;
class PointAnimation;
class PointAnimationUsingKeyFrames;
class PointKeyFrame;
class PointKeyFrameCollection;
class PolyBezierSegment;
class Polygon;
class Polyline;
class PolyLineSegment;
class PolyQuadraticBezierSegment;
class QuadraticBezierSegment;
class RadialGradientBrush;
class Rectangle;
class RectangleGeometry;
class ResourceDictionary;
class RotateTransform;
class RoutedEventArgs;
class RowDefinition;
class RowDefinitionCollection;
class Run;
class ScaleTransform;
class SelectionChangedEventArgs;
class Setter;
class SetterBase;
class SetterBaseCollection;
class Shape;
class SizeChangedEventArgs;
class SkewTransform;
class SolidColorBrush;
class SplineColorKeyFrame;
class SplineDoubleKeyFrame;
class SplinePointKeyFrame;
class StackPanel;
class Storyboard;
class Stroke;
class StrokeCollection;
class Style;
class StylusInfo;
class StylusPoint;
class StylusPointCollection;
class Surface;
class SystemTimeSource;
class TextBlock;
class TextBox;
class TextChangedEventArgs;
class TileBrush;
class Timeline;
class TimelineCollection;
class TimelineGroup;
class TimelineMarker;
class TimelineMarkerCollection;
class TimeManager;
class TimeSource;
class Transform;
class TransformCollection;
class TransformGroup;
class TranslateTransform;
class TriggerCollection;
class TriggerAction;
class TriggerActionCollection;
class TriggerBase;
class UIElement;
class UIElementCollection;
class UserControl;
class VideoBrush;
class VisualBrush;


#define checked_get_exact(kind, errval, mem)  g_return_val_if_fail (k == (kind), errval); return mem;
#define checked_get_subclass(kind, castas)  if (u.dependency_object == NULL) return NULL; g_return_val_if_fail (Type::Find((kind))->IsSubclassOf(k) || Type::Find(k)->IsSubclassOf((kind)), NULL); return (castas*)u.dependency_object;

struct Value {
public:
	void Init ();

	Value ();
	Value (const Value& v);
	explicit Value (Type::Kind k);
	explicit Value (bool z);
	explicit Value (double d);
	explicit Value (gint32 i);
	Value (gint64 i, Type::Kind as); // Use for TimeSpan and int64 values.
	Value (Color c);
	Value (EventObject *obj);
	Value (Point pt);
	Value (Rect rect);
	Value (Size size);
	Value (RepeatBehavior repeat);
	Value (Duration duration);
	Value (KeyTime keytime);
	Value (GridLength grid_length);
	Value (Thickness thickness);
	Value (CornerRadius radius);
	Value (const char *s);
	Value (Point *points, int count);
	Value (double *values, int count);

	~Value ();

	// Use these to create Values with dependency objects with
	// a reference count of 1 (giving the ownership of the object
	// to Value).
	// Useful in cases like this:
	//   SetValue (SomeProperty, new DependencyObject ())
	// in which case we have to unref the newly created object.
	static Value* CreateUnrefPtr (DependencyObject* dob);
	static Value  CreateUnref (DependencyObject* dob);

	bool operator!= (const Value &v) const;
	bool operator== (const Value &v) const;
	
	bool		Is (Types* additional_types, Type::Kind type) { return Type::IsSubclassOf (additional_types, k, type); }
	bool		Is (Type::Kind type) { return Is (NULL, type); }

	bool		AsBool ()	{ checked_get_exact (Type::BOOL, false, (bool)u.i32); }
	double 		AsDouble ()	{ checked_get_exact (Type::DOUBLE, 0.0, u.d); }
	guint64		AsUint64 ()	{ checked_get_exact (Type::UINT64, 0, u.ui64); }
	gint64		AsInt64 ()	{ checked_get_exact (Type::INT64, 0, u.i64); }
	TimeSpan	AsTimeSpan ()	{ checked_get_exact (Type::TIMESPAN, 0, (TimeSpan)u.i64); }
	gint32		AsInt32 ()	{ checked_get_exact (Type::INT32, 0, u.i32); }
	Color*		AsColor ()	{ checked_get_exact (Type::COLOR, NULL, u.color); }
	Point*		AsPoint ()	{ checked_get_exact (Type::POINT, NULL, u.point); }
	Rect*		AsRect ()	{ checked_get_exact (Type::RECT, NULL, u.rect); }
	Size*		AsSize ()	{ checked_get_exact (Type::SIZE, NULL, u.size); }
	char*		AsString ()	{ checked_get_exact (Type::STRING, NULL, u.s); }
	
	void *		AsManagedObject () { checked_get_exact (Type::MANAGED, NULL, u.managed_object); }
	
	RepeatBehavior*	AsRepeatBehavior ()	{ checked_get_exact (Type::REPEATBEHAVIOR, NULL, u.repeat); }
	Duration*	AsDuration ()	{ checked_get_exact (Type::DURATION, NULL, u.duration); }
	KeyTime*	AsKeyTime ()	{ checked_get_exact (Type::KEYTIME, NULL, u.keytime); }
	GridLength*     AsGridLength () { checked_get_exact (Type::GRIDLENGTH, NULL, u.grid_length); }
	Thickness*      AsThickness () { checked_get_exact (Type::THICKNESS, NULL, u.thickness); }
	CornerRadius*   AsCornerRadius () { checked_get_exact (Type::CORNERRADIUS, NULL, u.corner); }
	/* nullable primitives (all but bool) */
	double*		AsNullableDouble ()	{ checked_get_exact (Type::DOUBLE, NULL, &u.d); }
	guint64*	AsNullableUint64 ()	{ checked_get_exact (Type::UINT64, NULL, &u.ui64); }
	gint64* 	AsNullableInt64 ()	{ checked_get_exact (Type::INT64, NULL, &u.i64); }
	gint32* 	AsNullableInt32 ()	{ checked_get_exact (Type::INT32, NULL, &u.i32); }
	
	Animation*                     AsAnimation () { checked_get_subclass (Type::ANIMATION, Animation) }
	AnimationClock*                AsAnimationClock () { checked_get_subclass (Type::ANIMATIONCLOCK, AnimationClock) }
	Application*                   AsApplication () { checked_get_subclass (Type::APPLICATION, Application) }
	ArcSegment*                    AsArcSegment () { checked_get_subclass (Type::ARCSEGMENT, ArcSegment) }
	AssemblyPart*                  AsAssemblyPart () { checked_get_subclass (Type::ASSEMBLYPART, AssemblyPart) }
	AssemblyPartCollection*        AsAssemblyPartCollection () { checked_get_subclass (Type::ASSEMBLYPART_COLLECTION, AssemblyPartCollection) }
	BeginStoryboard*               AsBeginStoryboard () { checked_get_subclass (Type::BEGINSTORYBOARD, BeginStoryboard) }
	BezierSegment*                 AsBezierSegment () { checked_get_subclass (Type::BEZIERSEGMENT, BezierSegment) }
	BindingExpression*             AsBindingExpression () { checked_get_subclass (Type::BINDINGEXPRESSION, BindingExpression) }
	BindingExpressionBase*         AsBindingExpressionBase () { checked_get_subclass (Type::BINDINGEXPRESSIONBASE, BindingExpressionBase) }
	BitmapImage*                   AsBitmapImage () { checked_get_subclass (Type::BITMAPIMAGE, BitmapImage) }
	Border*                        AsBorder () { checked_get_subclass (Type::BORDER, Border) }
	Brush*                         AsBrush () { checked_get_subclass (Type::BRUSH, Brush) }
	Canvas*                        AsCanvas () { checked_get_subclass (Type::CANVAS, Canvas) }
	Clock*                         AsClock () { checked_get_subclass (Type::CLOCK, Clock) }
	ClockGroup*                    AsClockGroup () { checked_get_subclass (Type::CLOCKGROUP, ClockGroup) }
	Collection*                    AsCollection () { checked_get_subclass (Type::COLLECTION, Collection) }
	ColorAnimation*                AsColorAnimation () { checked_get_subclass (Type::COLORANIMATION, ColorAnimation) }
	ColorAnimationUsingKeyFrames*  AsColorAnimationUsingKeyFrames () { checked_get_subclass (Type::COLORANIMATIONUSINGKEYFRAMES, ColorAnimationUsingKeyFrames) }
	ColorKeyFrame*                 AsColorKeyFrame () { checked_get_subclass (Type::COLORKEYFRAME, ColorKeyFrame) }
	ColorKeyFrameCollection*       AsColorKeyFrameCollection () { checked_get_subclass (Type::COLORKEYFRAME_COLLECTION, ColorKeyFrameCollection) }
	ColumnDefinition*              AsColumnDefinition () { checked_get_subclass (Type::COLUMNDEFINITION, ColumnDefinition) }
	ColumnDefinitionCollection*    AsColumnDefinitionCollection () { checked_get_subclass (Type::COLUMNDEFINITION_COLLECTION, ColumnDefinitionCollection) }
	ContentControl*                AsContentControl () { checked_get_subclass (Type::CONTENTCONTROL, ContentControl) }
	Control*                       AsControl () { checked_get_subclass (Type::CONTROL, Control) }
	ControlTemplate*               AsControlTemplate () { checked_get_subclass (Type::CONTROLTEMPLATE, ControlTemplate) }
	DeepZoomImageTileSource*       AsDeepZoomImageTileSource () { checked_get_subclass (Type::DEEPZOOMIMAGETILESOURCE, DeepZoomImageTileSource) }
	DependencyObject*              AsDependencyObject () { checked_get_subclass (Type::DEPENDENCY_OBJECT, DependencyObject) }
	DependencyObjectCollection*    AsDependencyObjectCollection () { checked_get_subclass (Type::DEPENDENCY_OBJECT_COLLECTION, DependencyObjectCollection) }
	Deployment*                    AsDeployment () { checked_get_subclass (Type::DEPLOYMENT, Deployment) }
	DiscreteColorKeyFrame*         AsDiscreteColorKeyFrame () { checked_get_subclass (Type::DISCRETECOLORKEYFRAME, DiscreteColorKeyFrame) }
	DiscreteDoubleKeyFrame*        AsDiscreteDoubleKeyFrame () { checked_get_subclass (Type::DISCRETEDOUBLEKEYFRAME, DiscreteDoubleKeyFrame) }
	DiscreteObjectKeyFrame*        AsDiscreteObjectKeyFrame () { checked_get_subclass (Type::DISCRETEOBJECTKEYFRAME, DiscreteObjectKeyFrame) }
	DiscretePointKeyFrame*         AsDiscretePointKeyFrame () { checked_get_subclass (Type::DISCRETEPOINTKEYFRAME, DiscretePointKeyFrame) }
	DoubleCollection*              AsDoubleCollection () { checked_get_subclass (Type::DOUBLE_COLLECTION, DoubleCollection) }
	DoubleAnimation*               AsDoubleAnimation () { checked_get_subclass (Type::DOUBLEANIMATION, DoubleAnimation) }
	DoubleAnimationUsingKeyFrames* AsDoubleAnimationUsingKeyFrames () { checked_get_subclass (Type::DOUBLEANIMATIONUSINGKEYFRAMES, DoubleAnimationUsingKeyFrames) }
	DoubleKeyFrame*                AsDoubleKeyFrame () { checked_get_subclass (Type::DOUBLEKEYFRAME, DoubleKeyFrame) }
	DoubleKeyFrameCollection*      AsDoubleKeyFrameCollection () { checked_get_subclass (Type::DOUBLEKEYFRAME_COLLECTION, DoubleKeyFrameCollection) }
	Downloader*                    AsDownloader () { checked_get_subclass (Type::DOWNLOADER, Downloader) }
	DrawingAttributes*             AsDrawingAttributes () { checked_get_subclass (Type::DRAWINGATTRIBUTES, DrawingAttributes) }
	Ellipse*                       AsEllipse () { checked_get_subclass (Type::ELLIPSE, Ellipse) }
	EllipseGeometry*               AsEllipseGeometry () { checked_get_subclass (Type::ELLIPSEGEOMETRY, EllipseGeometry) }
	ErrorEventArgs*                AsErrorEventArgs () { checked_get_subclass (Type::ERROREVENTARGS, ErrorEventArgs) }
	EventArgs*                     AsEventArgs () { checked_get_subclass (Type::EVENTARGS, EventArgs) }
	EventObject*                   AsEventObject () { checked_get_subclass (Type::EVENTOBJECT, EventObject) }
	EventTrigger*                  AsEventTrigger () { checked_get_subclass (Type::EVENTTRIGGER, EventTrigger) }
	Expression*                    AsExpression () { checked_get_subclass (Type::EXPRESSION, Expression) }
	FrameworkElement*              AsFrameworkElement () { checked_get_subclass (Type::FRAMEWORKELEMENT, FrameworkElement) }
	FrameworkTemplate*             AsFrameworkTemplate () { checked_get_subclass (Type::FRAMEWORKTEMPLATE, FrameworkTemplate) }
	GeneralTransform*              AsGeneralTransform () { checked_get_subclass (Type::GENERALTRANSFORM, GeneralTransform) }
	Geometry*                      AsGeometry () { checked_get_subclass (Type::GEOMETRY, Geometry) }
	GeometryCollection*            AsGeometryCollection () { checked_get_subclass (Type::GEOMETRY_COLLECTION, GeometryCollection) }
	GeometryGroup*                 AsGeometryGroup () { checked_get_subclass (Type::GEOMETRYGROUP, GeometryGroup) }
	Glyphs*                        AsGlyphs () { checked_get_subclass (Type::GLYPHS, Glyphs) }
	GradientBrush*                 AsGradientBrush () { checked_get_subclass (Type::GRADIENTBRUSH, GradientBrush) }
	GradientStop*                  AsGradientStop () { checked_get_subclass (Type::GRADIENTSTOP, GradientStop) }
	GradientStopCollection*        AsGradientStopCollection () { checked_get_subclass (Type::GRADIENTSTOP_COLLECTION, GradientStopCollection) }
	Grid*                          AsGrid () { checked_get_subclass (Type::GRID, Grid) }
	Image*                         AsImage () { checked_get_subclass (Type::IMAGE, Image) }
	ImageBrush*                    AsImageBrush () { checked_get_subclass (Type::IMAGEBRUSH, ImageBrush) }
	ImageErrorEventArgs*           AsImageErrorEventArgs () { checked_get_subclass (Type::IMAGEERROREVENTARGS, ImageErrorEventArgs) }
	InkPresenter*                  AsInkPresenter () { checked_get_subclass (Type::INKPRESENTER, InkPresenter) }
	Inline*                        AsInline () { checked_get_subclass (Type::INLINE, Inline) }
	InlineCollection*              AsInlineCollection () { checked_get_subclass (Type::INLINE_COLLECTION, InlineCollection) }
	KeyEventArgs*                  AsKeyEventArgs () { checked_get_subclass (Type::KEYEVENTARGS, KeyEventArgs) }
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
	ManualTimeSource*              AsManualTimeSource () { checked_get_subclass (Type::MANUALTIMESOURCE, ManualTimeSource) }
	MarkerReachedEventArgs*        AsMarkerReachedEventArgs () { checked_get_subclass (Type::MARKERREACHEDEVENTARGS, MarkerReachedEventArgs) }
	Matrix*                        AsMatrix () { checked_get_subclass (Type::MATRIX, Matrix) }
	MatrixTransform*               AsMatrixTransform () { checked_get_subclass (Type::MATRIXTRANSFORM, MatrixTransform) }
	MediaAttribute*                AsMediaAttribute () { checked_get_subclass (Type::MEDIAATTRIBUTE, MediaAttribute) }
	MediaAttributeCollection*      AsMediaAttributeCollection () { checked_get_subclass (Type::MEDIAATTRIBUTE_COLLECTION, MediaAttributeCollection) }
	MediaBase*                     AsMediaBase () { checked_get_subclass (Type::MEDIABASE, MediaBase) }
	MediaElement*                  AsMediaElement () { checked_get_subclass (Type::MEDIAELEMENT, MediaElement) }
	MediaErrorEventArgs*           AsMediaErrorEventArgs () { checked_get_subclass (Type::MEDIAERROREVENTARGS, MediaErrorEventArgs) }
	MouseEventArgs*                AsMouseEventArgs () { checked_get_subclass (Type::MOUSEEVENTARGS, MouseEventArgs) }
	MultiScaleImage*               AsMultiScaleImage () { checked_get_subclass (Type::MULTISCALEIMAGE, MultiScaleImage) }
	MultiScaleSubImage*            AsMultiScaleSubImage () { checked_get_subclass (Type::MULTISCALESUBIMAGE, MultiScaleSubImage) }
	MultiScaleTileSource*          AsMultiScaleTileSource () { checked_get_subclass (Type::MULTISCALETILESOURCE, MultiScaleTileSource) }
	NameScope*                     AsNameScope () { checked_get_subclass (Type::NAMESCOPE, NameScope) }
	ObjectAnimationUsingKeyFrames* AsObjectAnimationUsingKeyFrames () { checked_get_subclass (Type::OBJECTANIMATIONUSINGKEYFRAMES, ObjectAnimationUsingKeyFrames) }
	ObjectKeyFrame*                AsObjectKeyFrame () { checked_get_subclass (Type::OBJECTKEYFRAME, ObjectKeyFrame) }
	ObjectKeyFrameCollection*      AsObjectKeyFrameCollection () { checked_get_subclass (Type::OBJECTKEYFRAME_COLLECTION, ObjectKeyFrameCollection) }
	Panel*                         AsPanel () { checked_get_subclass (Type::PANEL, Panel) }
	ParallelTimeline*              AsParallelTimeline () { checked_get_subclass (Type::PARALLELTIMELINE, ParallelTimeline) }
	ParserErrorEventArgs*          AsParserErrorEventArgs () { checked_get_subclass (Type::PARSERERROREVENTARGS, ParserErrorEventArgs) }
	Path*                          AsPath () { checked_get_subclass (Type::PATH, Path) }
	PathFigure*                    AsPathFigure () { checked_get_subclass (Type::PATHFIGURE, PathFigure) }
	PathFigureCollection*          AsPathFigureCollection () { checked_get_subclass (Type::PATHFIGURE_COLLECTION, PathFigureCollection) }
	PathGeometry*                  AsPathGeometry () { checked_get_subclass (Type::PATHGEOMETRY, PathGeometry) }
	PathSegment*                   AsPathSegment () { checked_get_subclass (Type::PATHSEGMENT, PathSegment) }
	PathSegmentCollection*         AsPathSegmentCollection () { checked_get_subclass (Type::PATHSEGMENT_COLLECTION, PathSegmentCollection) }
	PointCollection*               AsPointCollection () { checked_get_subclass (Type::POINT_COLLECTION, PointCollection) }
	PointAnimation*                AsPointAnimation () { checked_get_subclass (Type::POINTANIMATION, PointAnimation) }
	PointAnimationUsingKeyFrames*  AsPointAnimationUsingKeyFrames () { checked_get_subclass (Type::POINTANIMATIONUSINGKEYFRAMES, PointAnimationUsingKeyFrames) }
	PointKeyFrame*                 AsPointKeyFrame () { checked_get_subclass (Type::POINTKEYFRAME, PointKeyFrame) }
	PointKeyFrameCollection*       AsPointKeyFrameCollection () { checked_get_subclass (Type::POINTKEYFRAME_COLLECTION, PointKeyFrameCollection) }
	PolyBezierSegment*             AsPolyBezierSegment () { checked_get_subclass (Type::POLYBEZIERSEGMENT, PolyBezierSegment) }
	Polygon*                       AsPolygon () { checked_get_subclass (Type::POLYGON, Polygon) }
	Polyline*                      AsPolyline () { checked_get_subclass (Type::POLYLINE, Polyline) }
	PolyLineSegment*               AsPolyLineSegment () { checked_get_subclass (Type::POLYLINESEGMENT, PolyLineSegment) }
	PolyQuadraticBezierSegment*    AsPolyQuadraticBezierSegment () { checked_get_subclass (Type::POLYQUADRATICBEZIERSEGMENT, PolyQuadraticBezierSegment) }
	QuadraticBezierSegment*        AsQuadraticBezierSegment () { checked_get_subclass (Type::QUADRATICBEZIERSEGMENT, QuadraticBezierSegment) }
	RadialGradientBrush*           AsRadialGradientBrush () { checked_get_subclass (Type::RADIALGRADIENTBRUSH, RadialGradientBrush) }
	Rectangle*                     AsRectangle () { checked_get_subclass (Type::RECTANGLE, Rectangle) }
	RectangleGeometry*             AsRectangleGeometry () { checked_get_subclass (Type::RECTANGLEGEOMETRY, RectangleGeometry) }
	ResourceDictionary*            AsResourceDictionary () { checked_get_subclass (Type::RESOURCE_DICTIONARY, ResourceDictionary) }
	RotateTransform*               AsRotateTransform () { checked_get_subclass (Type::ROTATETRANSFORM, RotateTransform) }
	RoutedEventArgs*               AsRoutedEventArgs () { checked_get_subclass (Type::ROUTEDEVENTARGS, RoutedEventArgs) }
	RowDefinition*                 AsRowDefinition () { checked_get_subclass (Type::ROWDEFINITION, RowDefinition) }
	RowDefinitionCollection*       AsRowDefinitionCollection () { checked_get_subclass (Type::ROWDEFINITION_COLLECTION, RowDefinitionCollection) }
	Run*                           AsRun () { checked_get_subclass (Type::RUN, Run) }
	ScaleTransform*                AsScaleTransform () { checked_get_subclass (Type::SCALETRANSFORM, ScaleTransform) }
	SelectionChangedEventArgs*     AsSelectionChangedEventArgs () { checked_get_subclass (Type::SELECTIONCHANGEDEVENTARGS, SelectionChangedEventArgs) }
	Setter*                        AsSetter () { checked_get_subclass (Type::SETTER, Setter) }
	SetterBase*                    AsSetterBase () { checked_get_subclass (Type::SETTERBASE, SetterBase) }
	SetterBaseCollection*          AsSetterBaseCollection () { checked_get_subclass (Type::SETTERBASE_COLLECTION, SetterBaseCollection) }
	Shape*                         AsShape () { checked_get_subclass (Type::SHAPE, Shape) }
	SizeChangedEventArgs*          AsSizeChangedEventArgs () { checked_get_subclass (Type::SIZECHANGEDEVENTARGS, SizeChangedEventArgs) }
	SkewTransform*                 AsSkewTransform () { checked_get_subclass (Type::SKEWTRANSFORM, SkewTransform) }
	SolidColorBrush*               AsSolidColorBrush () { checked_get_subclass (Type::SOLIDCOLORBRUSH, SolidColorBrush) }
	SplineColorKeyFrame*           AsSplineColorKeyFrame () { checked_get_subclass (Type::SPLINECOLORKEYFRAME, SplineColorKeyFrame) }
	SplineDoubleKeyFrame*          AsSplineDoubleKeyFrame () { checked_get_subclass (Type::SPLINEDOUBLEKEYFRAME, SplineDoubleKeyFrame) }
	SplinePointKeyFrame*           AsSplinePointKeyFrame () { checked_get_subclass (Type::SPLINEPOINTKEYFRAME, SplinePointKeyFrame) }
	StackPanel*                    AsStackPanel () { checked_get_subclass (Type::STACKPANEL, StackPanel) }
	Storyboard*                    AsStoryboard () { checked_get_subclass (Type::STORYBOARD, Storyboard) }
	Stroke*                        AsStroke () { checked_get_subclass (Type::STROKE, Stroke) }
	StrokeCollection*              AsStrokeCollection () { checked_get_subclass (Type::STROKE_COLLECTION, StrokeCollection) }
	Style*                         AsStyle () { checked_get_subclass (Type::STYLE, Style) }
	StylusInfo*                    AsStylusInfo () { checked_get_subclass (Type::STYLUSINFO, StylusInfo) }
	StylusPoint*                   AsStylusPoint () { checked_get_subclass (Type::STYLUSPOINT, StylusPoint) }
	StylusPointCollection*         AsStylusPointCollection () { checked_get_subclass (Type::STYLUSPOINT_COLLECTION, StylusPointCollection) }
	Surface*                       AsSurface () { checked_get_subclass (Type::SURFACE, Surface) }
	SystemTimeSource*              AsSystemTimeSource () { checked_get_subclass (Type::SYSTEMTIMESOURCE, SystemTimeSource) }
	TextBlock*                     AsTextBlock () { checked_get_subclass (Type::TEXTBLOCK, TextBlock) }
	TextBox*                       AsTextBox () { checked_get_subclass (Type::TEXTBOX, TextBox) }
	TextChangedEventArgs*          AsTextChangedEventArgs () { checked_get_subclass (Type::TEXTCHANGEDEVENTARGS, TextChangedEventArgs) }
	TileBrush*                     AsTileBrush () { checked_get_subclass (Type::TILEBRUSH, TileBrush) }
	Timeline*                      AsTimeline () { checked_get_subclass (Type::TIMELINE, Timeline) }
	TimelineCollection*            AsTimelineCollection () { checked_get_subclass (Type::TIMELINE_COLLECTION, TimelineCollection) }
	TimelineGroup*                 AsTimelineGroup () { checked_get_subclass (Type::TIMELINEGROUP, TimelineGroup) }
	TimelineMarker*                AsTimelineMarker () { checked_get_subclass (Type::TIMELINEMARKER, TimelineMarker) }
	TimelineMarkerCollection*      AsTimelineMarkerCollection () { checked_get_subclass (Type::TIMELINEMARKER_COLLECTION, TimelineMarkerCollection) }
	TimeManager*                   AsTimeManager () { checked_get_subclass (Type::TIMEMANAGER, TimeManager) }
	TimeSource*                    AsTimeSource () { checked_get_subclass (Type::TIMESOURCE, TimeSource) }
	Transform*                     AsTransform () { checked_get_subclass (Type::TRANSFORM, Transform) }
	TransformCollection*           AsTransformCollection () { checked_get_subclass (Type::TRANSFORM_COLLECTION, TransformCollection) }
	TransformGroup*                AsTransformGroup () { checked_get_subclass (Type::TRANSFORMGROUP, TransformGroup) }
	TranslateTransform*            AsTranslateTransform () { checked_get_subclass (Type::TRANSLATETRANSFORM, TranslateTransform) }
	TriggerCollection*             AsTriggerCollection () { checked_get_subclass (Type::TRIGGER_COLLECTION, TriggerCollection) }
	TriggerAction*                 AsTriggerAction () { checked_get_subclass (Type::TRIGGERACTION, TriggerAction) }
	TriggerActionCollection*       AsTriggerActionCollection () { checked_get_subclass (Type::TRIGGERACTION_COLLECTION, TriggerActionCollection) }
	TriggerBase*                   AsTriggerBase () { checked_get_subclass (Type::TRIGGERBASE, TriggerBase) }
	UIElement*                     AsUIElement () { checked_get_subclass (Type::UIELEMENT, UIElement) }
	UIElementCollection*           AsUIElementCollection () { checked_get_subclass (Type::UIELEMENT_COLLECTION, UIElementCollection) }
	UserControl*                   AsUserControl () { checked_get_subclass (Type::USERCONTROL, UserControl) }
	VideoBrush*                    AsVideoBrush () { checked_get_subclass (Type::VIDEOBRUSH, VideoBrush) }
	VisualBrush*                   AsVisualBrush () { checked_get_subclass (Type::VISUALBRUSH, VisualBrush) }

	
	char *ToString ();
	
  	Type::Kind GetKind ();
	
	void FreeValue ();
	
  private:
	Type::Kind k;
	
	gint32 padding;
	
	union {
		double d;
		guint64 ui64;
		gint64 i64;
		gint32 i32;
		char *s;
		EventObject *dependency_object;
		Color *color;
		Point *point;
		Rect *rect;
		Size *size;
		RepeatBehavior *repeat;
		Duration *duration;
		KeyTime *keytime;
		GridLength *grid_length;
		Thickness *thickness;
		CornerRadius *corner;
		void *managed_object;
	} u;
	
	// You don't want to be using this ctor.  it's here to help
	// c++ recognize bad unspecified pointer args to Value ctors
	// (it normally converts them to bool, which we handle, so you
	// never see the error of your ways).  So do the world a
	// favor, and don't expose this ctor. :)
	Value (void *v) { }
	// You don't want to be using this ctor either.
	// Use the Value (gint64, Type::Kind) ctor
	// (Both for TimeSpan and int64_t)
	// This one is bad because it get used by either int64_t
	// and TimeSpan, and the constructor doesn't know which 
	// of the two types it is.
	explicit Value (gint64 i) {};
};

G_BEGIN_DECLS
void value_free_value (Value *value);
G_END_DECLS


#endif /* MOON_VALUE_H */

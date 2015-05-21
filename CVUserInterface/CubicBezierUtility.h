
/**
 *
 * Utility class for rendering bezier curve:
 *		- calculate klm
 *		- quad triangulation
 *		- curve type
 *		- etc.
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __Cubic_Bezier_Utility__
#define __Cubic_Bezier_Utility__

#include <vector>

#include "MyVertex.h"
#include "MyCurveTriangle.h"
#include "CurveType.h"

class CubicBezierUtility
{
public:	

	// nasty thing here...
	// Todo: make these four variables inside function parameters
	static int startIndex;
	static int endIndex;
	static CVSystem::MyPoint startPoint;
	static CVSystem::MyPoint endPoint;

	// calculate klm
	static void ComputeCubic(double x0, double y0, 
							 double x1, double y1, 
							 double x2, double y2, 
							 double x3, double y3,
							 std::vector<MyCurveTriangle>& bzTriangles,
							 std::vector<MyCurveTriangle>& piTriangles,
							 std::vector<MyCurveTriangle>& poTriangles,
							 int recursiveType = -1);

	// triangulate quad
	static void Triangulation (double  x0, double  y0, 
							   double  x1, double  y1, 
							   double  x2, double  y2, 
							   double  x3, double  y3,
							   std::vector<MyCurveTriangle>& bzTriangles,
							   std::vector<double> klm);

	// add a bezier triangle to list
	static void AddBezierTriangle(MyVertex v0, MyVertex v1, MyVertex v2, std::vector<MyCurveTriangle>& bzTriangles);

	// add solid shading triangle to list
	static void AddPlainTriangle(CVSystem::MyPoint p1, CVSystem::MyPoint p2, CVSystem::MyPoint p3, std::vector<MyCurveTriangle>& piTriangles, std::vector<MyCurveTriangle>& poTriangles);

	// find out the type of curve (loop, serpentine, cusp, ...)
	static CurveType DetermineType (CVSystem::MyPoint v0, CVSystem::MyPoint v1, CVSystem::MyPoint v2, CVSystem::MyPoint v3, double& d0, double& d1, double& d2, double& d3);
	
	// draw bezier triangle (shader is already bind)
	static void DrawBezierTriangle (double x0, double y0, double x1, double y1, double x2, double y2, double st0, double st1, double st2, double st3, double st4, double st5, double st6, double st7, double st8);
	
	// draw solid shading triangle
	static void DrawPlainTriangle (double x0, double y0, double x1, double y1, double x2, double y2, bool inside);
		
	// Utilities
	// are two points equal ?
	static bool ApproxEqual(CVSystem::MyPoint& v0, CVSystem::MyPoint& v1);
	// is a point inside a triangle
	static bool PointInTriangle(CVSystem::MyPoint& point, CVSystem::MyPoint& a, CVSystem::MyPoint& b, CVSystem::MyPoint& c);
	// orientation of triangle polygon
	static int	Orientation(CVSystem::MyPoint& p1, CVSystem::MyPoint& p2, CVSystem::MyPoint& p3);
	// do two line intersect ?
	static bool LinesIntersect(CVSystem::MyPoint& p1, CVSystem::MyPoint& q1, CVSystem::MyPoint& p2, CVSystem::MyPoint& q2);
	// orientation of triangle polygon
	static bool IsClockwise (double x0, double y0, double x1, double y1, double x2, double y2);

};

#endif


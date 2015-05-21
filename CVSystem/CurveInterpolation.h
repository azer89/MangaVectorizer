
/**
 * Bezier curve interpolation using DeCasteljau
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __Curve_Interpolation__
#define __Curve_Interpolation__

#include <vector>
#include "MyPoint.h"

namespace CVSystem
{
	class CurveInterpolation
	{
	public:

		// recursive de casteljau
		static void DeCasteljau(std::vector<MyPoint>& poly, MyPoint p0, MyPoint p1, MyPoint p2, MyPoint p3, double subdivide_limit);

		// mid point de casteljau (non recursive)
		static MyPoint DeCasteljauMidPoint(MyPoint p0, MyPoint p1, MyPoint p2, MyPoint p3);

		// point interpolation in between two points
		static void PointInterpolation(std::vector<MyPoint>& poly, MyPoint pt1, MyPoint pt2, double f, double subLimit);
	};
}

#endif 



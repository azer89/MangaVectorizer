
/**
 *
 * Ramer-Douglas-Peucker
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __Curve_RDP__
#define __Curve_RDP__

#include <vector>

#include "MyPoint.h"

namespace CVSystem
{
	// class for Ramer-Douglas-Peucker
	class CurveRDP
	{
	public:
		// distance a point to a line
		static double PerpendicularDistance(MyPoint p, MyPoint p1, MyPoint p2) ;

		// RDP flags
		static void RDP(std::vector<bool>& pointFlags, std::vector<MyPoint> points, double epsilon, int startIndex, int endIndex, double rdp_point_min);

		// RDP partitioning
		static void SimplifyRDP(std::vector<MyPoint>& oldCurves, std::vector<MyPoint>& newCurves, double epsilon);

		// RDP partitioning
		static void SimplifyRDPRecursive(std::vector<MyPoint>& oldCurves, std::vector<MyPoint>& newCurves, double epsilon, int startIndex, int endIndex);
	};
}

#endif

/**
 *
 * Data structure for bezier curves
 * for every curve C, C = {b0, b1, b2, b3}
 * This data structure doesn't store b0 and b1
 * since it is indexed
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __My_Indexed_Bezier_Curves__
#define __My_Indexed_Bezier_Curves__

#include "MyPoint.h"

namespace CVSystem
{
	
	struct MyIndexedBezierCurves
	{	
	public:
		// start point : 0
		// end point   : n - 1
		//
		// beware of ineffeciency, last points == start point due to weird implementation
		std::vector<int>	 indices;

		std::vector<MyPoint> p0_points;		// b0 (temporary)
		std::vector<MyPoint> p1_points;		// b1
		std::vector<MyPoint> p2_points;		// b2
											// b3 is the next point

		// corner flags (currently is still disabled)
		std::vector<bool>	 cornerFlags;

		// orientation
		bool isClockwise;

		// is closed curve?
		bool IsClosed(std::vector<MyPoint> vertexList)
		{
			if(indices.size() <= 1) {return false;}

			MyPoint pA = vertexList[indices[0]];
			MyPoint pB = vertexList[indices[indices.size() - 1]];

			double dist = pA.Distance(pB);
			if(dist > -M_EPS && dist < M_EPS) {return true;}
			return false;
		}

		// is valid?
		bool ArePointsValid(std::vector<CVSystem::MyPoint> vertexList, int idx)
		{
			CVSystem::MyPoint realStartPoint = vertexList[idx];
			CVSystem::MyPoint realEndPoint   = vertexList[idx + 1];

			CVSystem::MyPoint startPoint = p0_points[idx];
			CVSystem::MyPoint endPoint   = p0_points[idx + 1];

			if(realStartPoint.Distance(startPoint) < M_EPS && realEndPoint.Distance(endPoint) < M_EPS )
			{ return true; }

			return false;
		}

		// orientation of a polygon
		void CalculateOrientation(std::vector<CVSystem::MyPoint> vertexList)
		{
			double sumValue = 0;
			for(size_t a = 0; a < indices.size() - 1; a++)
			{
				double x1 = vertexList[indices[a]].x;
				double y1 = vertexList[indices[a]].y;
				double x2 = vertexList[indices[a+1]].x;
				double y2 = vertexList[indices[a+1]].y;

				sumValue += ((x2 - x1) * (y2 + y1));
			}

			// because the cartesian coordinate is inverted
			if(sumValue >= 0) { isClockwise = false; }
			else { isClockwise = true; }
		}
	};
}
#endif
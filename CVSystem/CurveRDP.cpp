
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "CurveRDP.h"

double CVSystem::CurveRDP::PerpendicularDistance(CVSystem::MyPoint p, CVSystem::MyPoint p1, CVSystem::MyPoint p2) 
{
	// if start and end point are on the same x the distance is the difference in X.
	double result;
	if (abs(p1.x - p2.x) < M_EPS) { result = abs(p.x - p1.x); }
	else
	{
		double slope = (p2.y - p1.y) / (p2.x - p1.x);
		double intercept = p1.y - (slope * p1.x);
		result = abs(slope * p.x - p.y + intercept) / sqrt(pow(slope, 2) + 1);
	}

	return result;
}

void CVSystem::CurveRDP::RDP(std::vector<bool>& flags, std::vector<CVSystem::MyPoint> points, double epsilon, int startIndex, int endIndex, double rdp_point_min)
{			
	//if (endIndex - startIndex < rdp_point_min)
	//	{ return; }

	MyPoint firstPoint = points[startIndex];
	MyPoint lastPoint=points[endIndex];

	int index = -1;
	double dist = DBL_MIN;
	for (int i = startIndex + 1; i < endIndex; i++)
	{
		double cDist = PerpendicularDistance(points[i], firstPoint, lastPoint);
		if (cDist > dist)
		{
			dist = cDist;
			index = i;
		}
	}

	if (dist > epsilon)
	{
		if(index - startIndex >= rdp_point_min && endIndex - index >= rdp_point_min)
		{
			// Todo: wrong recursive sequence
			flags[index] = true;
			RDP(flags, points, epsilon, startIndex, index, rdp_point_min);
			RDP(flags, points, epsilon, index, endIndex  , rdp_point_min);
		}

		/*flags[index] = true;
		RDP(flags, points, epsilon, startIndex, index, rdp_point_min);
		RDP(flags, points, epsilon, index, endIndex  , rdp_point_min);*/
	}
}

void CVSystem::CurveRDP::SimplifyRDP(std::vector<CVSystem::MyPoint>& oldCurves, std::vector<CVSystem::MyPoint>& newCurves, double epsilon)
{
	newCurves.clear();
	newCurves.push_back(oldCurves[0]);
	SimplifyRDPRecursive(oldCurves, newCurves, epsilon, 0, oldCurves.size() - 1);
	newCurves.push_back(oldCurves[oldCurves.size() - 1]);
}
void CVSystem::CurveRDP::SimplifyRDPRecursive(std::vector<CVSystem::MyPoint>& oldCurves, std::vector<CVSystem::MyPoint>& newCurves, double epsilon, int startIndex, int endIndex)
{
	MyPoint firstPoint = oldCurves[startIndex];
	MyPoint lastPoint = oldCurves[endIndex];

	int index = -1;
	double dist = DBL_MIN;
	for (int i = startIndex + 1; i < endIndex; i++)
	{
		double cDist = PerpendicularDistance(oldCurves[i], firstPoint, lastPoint);
		if (cDist > dist)
		{
			dist = cDist;
			index = i;
		}
	}

	if (index != -1 && dist > epsilon)
	{
		SimplifyRDPRecursive(oldCurves, newCurves, epsilon, startIndex, index);
		newCurves.push_back(oldCurves[index]);		
		SimplifyRDPRecursive(oldCurves, newCurves, epsilon, index, endIndex);
	}
}

/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "CurveInterpolation.h"

void CVSystem::CurveInterpolation::PointInterpolation(std::vector<MyPoint>& poly, MyPoint pt1, MyPoint pt2, double f, double subLimit)
{
	if(pt1.Distance(pt2) <= subLimit)
	{
		poly.push_back(pt1);
	}
	else
	{
		MyPoint newPt = pt1 + (pt2 - pt1) * f;
		PointInterpolation(poly, pt1, newPt, f, subLimit);
		PointInterpolation(poly, newPt, pt2, f, subLimit);
	}
}

CVSystem::MyPoint CVSystem::CurveInterpolation::DeCasteljauMidPoint(MyPoint p0, MyPoint p1, MyPoint p2, MyPoint p3)
{
	double splitParam = 0.5;	// split into two equal parts

	double x0 = p0.x; double y0 = p0.y;
	double x1 = p1.x; double y1 = p1.y;
	double x2 = p2.x; double y2 = p2.y; 
	double x3 = p3.x; double y3 = p3.y;

	double x01 = (x1 - x0) * splitParam + x0;		double x12 = (x2 - x1) * splitParam + x1;		double x23 = (x3 - x2) * splitParam + x2;
	double y01 = (y1 - y0) * splitParam + y0;		double y12 = (y2 - y1) * splitParam + y1;		double y23 = (y3 - y2) * splitParam + y2;		

	double x012 = (x12 - x01) * splitParam + x01;	double x123 = (x23 - x12) * splitParam + x12;
	double y012 = (y12 - y01) * splitParam + y01;	double y123 = (y23 - y12) * splitParam + y12;

	double x0123 = (x123 - x012) * splitParam + x012;
	double y0123 = (y123 - y012) * splitParam + y012;

	return MyPoint(x0123, y0123);
}


void CVSystem::CurveInterpolation::DeCasteljau(std::vector<MyPoint>& poly, MyPoint p0, MyPoint p1, MyPoint p2, MyPoint p3, double subdivide_limit)
{
	if(p0.Distance(p3) <= subdivide_limit)
	{
		poly.push_back(p0);
	}
	else
	{
		double splitParam = 0.5;	// split into two equal parts

		double x0 = p0.x; double y0 = p0.y;
		double x1 = p1.x; double y1 = p1.y;
		double x2 = p2.x; double y2 = p2.y; 
		double x3 = p3.x; double y3 = p3.y;

		double x01 = (x1 - x0) * splitParam + x0;		double x12 = (x2 - x1) * splitParam + x1;		double x23 = (x3 - x2) * splitParam + x2;
		double y01 = (y1 - y0) * splitParam + y0;		double y12 = (y2 - y1) * splitParam + y1;		double y23 = (y3 - y2) * splitParam + y2;		

		double x012 = (x12 - x01) * splitParam + x01;	double x123 = (x23 - x12) * splitParam + x12;
		double y012 = (y12 - y01) * splitParam + y01;	double y123 = (y23 - y12) * splitParam + y12;

		double x0123 = (x123 - x012) * splitParam + x012;
		double y0123 = (y123 - y012) * splitParam + y012;

		DeCasteljau(poly, MyPoint(x0, y0),        MyPoint(x01, y01),   MyPoint(x012, y012), MyPoint(x0123, y0123), subdivide_limit);
		DeCasteljau(poly, MyPoint(x0123,  y0123), MyPoint(x123, y123), MyPoint(x23, y23),   MyPoint(x3, y3),	   subdivide_limit);		
	}
}
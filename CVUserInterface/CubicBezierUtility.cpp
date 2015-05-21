
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "CubicBezierUtility.h"
#include "SystemParams.h"

// nasty thing here...
// Todo: make these four variables inside function parameters
int CubicBezierUtility::startIndex = -1;
int CubicBezierUtility::endIndex = -1;
CVSystem::MyPoint CubicBezierUtility::startPoint = CVSystem::MyPoint();
CVSystem::MyPoint CubicBezierUtility::endPoint = CVSystem::MyPoint();


void CubicBezierUtility::ComputeCubic(double x0, double y0, 
									  double x1, double y1, 
									  double x2, double y2, 
									  double x3, double y3,
									  std::vector<MyCurveTriangle>& bzTriangles,
									  std::vector<MyCurveTriangle>& piTriangles,
									  std::vector<MyCurveTriangle>& poTriangles,
									  int recursiveType)
{
	double d0 = 0; double d1 = 0; double d2 = 0; double d3 = 0;
	CurveType curve_type = DetermineType (CVSystem::MyPoint(x0, y0),
										CVSystem::MyPoint(x1, y1),
										CVSystem::MyPoint(x2, y2),
										CVSystem::MyPoint(x3, y3),
										d0, d1, d2, d3);

	double OneThird = 1.0f / 3.0f;
	double TwoThirds = 2.0f / 3.0f;

	double t1;
	double ls;
	double lt;
	double ms;
	double mt;

	double ltMinusLs;
	double mtMinusMs;
	double lsMinusLt;
	double ql;
	double qm;

	// bool flip
	bool flip = false;

	// artifact on loop
	int errorLoop = -1;
	double splitParam = 0;

	std::vector<double> klm(12);

	switch (curve_type)
	{
	case CURVE_TYPE_UNKNOWN:
		break;

	case CURVE_TYPE_SERPENTINE:	

		t1 = sqrtf(9.0f * d2 * d2 - 12 * d1 * d3);
		ls = 3.0f * d2 - t1;
		lt = 6.0f * d1;
		ms = 3.0f * d2 + t1;
		mt = lt;
		ltMinusLs = lt - ls;
		mtMinusMs = mt - ms;

		klm[0] =  ls * ms;
		klm[1] =  	ls * ls * ls;
		klm[2] =  	ms * ms * ms;

		klm[3] =  OneThird * (3.0f * ls * ms - ls * mt - lt * ms);
		klm[4]  = 	ls * ls * (ls - lt);
		klm[5]  = 	ms * ms * (ms - mt);

		klm[6] =  OneThird * (lt * (mt - 2.0f * ms) + ls * (3.0f * ms - 2.0f * mt));
		klm[7] =  	ltMinusLs * ltMinusLs * ls;
		klm[8] =  	mtMinusMs * mtMinusMs * ms;

		klm[9] =  ltMinusLs * mtMinusMs;
		klm[10] =  	-(ltMinusLs * ltMinusLs * ltMinusLs);
		klm[11]  = 	-(mtMinusMs * mtMinusMs * mtMinusMs);

		if (d1 < 0.0f)
			flip = true;

		break;

	case CURVE_TYPE_LOOP:

		t1 = sqrtf(4.0f * d1 * d3 - 3.0f * d2 * d2);
		ls = d2 - t1;
		lt = 2.0f * d1;
		ms = d2 + t1;
		mt = lt;

		// Figure out whether there is a rendering artifact requiring
		// the curve to be subdivided by the caller.
		ql = ls / lt;
		qm = ms / mt;

		if (0.0f < ql && ql < 1.0f) 
		{
			errorLoop = 1;
			splitParam = ql;
		}
		else if (0.0f < qm && qm < 1.0f) 
		{
			errorLoop = 2;
			splitParam = qm;
		}

		ltMinusLs = lt - ls;
		mtMinusMs = mt - ms;
		klm[0] =  ls * ms;
		klm[1] =  	ls * ls * ms;
		klm[2] =  	ls * ms * ms;

		klm[3] =  OneThird * (-ls * mt - lt * ms + 3.0f * ls * ms);
		klm[4] =  	-OneThird * ls * (ls * (mt - 3.0f * ms) + 2.0f * lt * ms);
		klm[5] =  	-OneThird * ms * (ls * (2.0f * mt - 3.0f * ms) + lt * ms);

		klm[6] =  OneThird * (lt * (mt - 2.0f * ms) + ls * (3.0f * ms - 2.0f * mt));
		klm[7] =  	OneThird * (lt - ls) * (ls * (2.0f * mt - 3.0f * ms) + lt * ms);
		klm[8] =  	OneThird * (mt - ms) * (ls * (mt - 3.0f * ms) + 2.0f * lt * ms);

		klm[9] =  ltMinusLs * mtMinusMs;
		klm[10] =  	-(ltMinusLs * ltMinusLs) * mtMinusMs;
		klm[11] =  	-ltMinusLs * mtMinusMs * mtMinusMs;

		if(recursiveType == -1)
			flip =  ((d1 > 0.0f && klm[0] < 0.0f) || (d1 < 0.0f && klm[0] > 0.0f));

		break;

	case CURVE_TYPE_CUSP:
		ls = d3;
		lt = 3.0f * d2;
		lsMinusLt = ls - lt;
		klm[0] =  ls;
		klm[1] =  ls * ls * ls;
		klm[2] =  1.0f;

		klm[3] =  ls - OneThird * lt;
		klm[4] =  	ls * ls * lsMinusLt;
		klm[5] =  	1.0f;

		klm[6] =  ls - TwoThirds * lt;
		klm[7] =  	lsMinusLt * lsMinusLt * ls;
		klm[8] =  	1.0f;

		klm[9] =  lsMinusLt;
		klm[10] =  	lsMinusLt * lsMinusLt * lsMinusLt;
		klm[11] =  	1.0f;

		break;

	case CURVE_TYPE_QUADRATIC:
		klm[0] =  0;
		klm[1] =  0;
		klm[2] =  0;

		klm[3] =  OneThird;
		klm[4] =  0;
		klm[5] =  OneThird;

		klm[6] =  TwoThirds;
		klm[7] =  OneThird;
		klm[8] =  TwoThirds;

		klm[9] =  1;
		klm[10] = 1;
		klm[11] = 1;

		if (d3 < 0)
			flip = true;

		break;

	case CURVE_TYPE_LINE:
		break;
	}

	if(errorLoop != -1 && recursiveType == -1)
	{
		double x01 = (x1 - x0) * splitParam + x0;		double x12 = (x2 - x1) * splitParam + x1;		double x23 = (x3 - x2) * splitParam + x2;
		double y01 = (y1 - y0) * splitParam + y0;		double y12 = (y2 - y1) * splitParam + y1;		double y23 = (y3 - y2) * splitParam + y2;		

		double x012 = (x12 - x01) * splitParam + x01;	double x123 = (x23 - x12) * splitParam + x12;
		double y012 = (y12 - y01) * splitParam + y01;	double y123 = (y23 - y12) * splitParam + y12;

		double x0123 = (x123 - x012) * splitParam + x012;
		double y0123 = (y123 - y012) * splitParam + y012;

		AddPlainTriangle(CVSystem::MyPoint(x0, y0), CVSystem::MyPoint(x0123, y0123), CVSystem::MyPoint(x3, y3), piTriangles, poTriangles);

		if(errorLoop == 1)	// flip second
		{
			ComputeCubic(x0, y0, x01, y01, x012, y012, x0123, y0123, bzTriangles, piTriangles, poTriangles, 0);
			ComputeCubic(x0123,  y0123, x123, y123, x23, y23, x3, y3,bzTriangles, piTriangles, poTriangles, 1);
		}
		else if(errorLoop == 2) // flip first
		{
			ComputeCubic(x0, y0, x01, y01, x012, y012, x0123, y0123, bzTriangles, piTriangles, poTriangles, 1);
			ComputeCubic(x0123,  y0123, x123, y123, x23, y23, x3, y3, bzTriangles, piTriangles, poTriangles, 0);
		}

		return;
	}

	if(recursiveType == 1) { flip = !flip; }

	if(flip)
	{
		klm[0] = -klm[0]; klm[1] = -klm[1];
		klm[3] = -klm[3]; klm[4] = -klm[4];
		klm[6] = -klm[6]; klm[7] = -klm[7];
		klm[9] = -klm[9]; klm[10] = -klm[10];
	}

	Triangulation (x0, y0, x1, y1, x2, y2, x3, y3, bzTriangles, klm);
}

void CubicBezierUtility::Triangulation (double  x0, double  y0, 
										double  x1, double  y1, 
										double  x2, double  y2, 
										double  x3, double  y3,
										std::vector<MyCurveTriangle>& bzTriangles,
										std::vector<double> klm)
{
	std::vector<MyVertex> vertices;
	vertices.push_back(MyVertex(x0, y0, klm[0], klm[1], klm[2]));
	vertices.push_back(MyVertex(x1, y1, klm[3], klm[4], klm[5]));
	vertices.push_back(MyVertex(x2, y2, klm[6], klm[7], klm[8]));
	vertices.push_back(MyVertex(x3, y3, klm[9], klm[10], klm[11]));

	// First test for degenerate cases.
	for (int i = 0; i < 4; ++i) 
	{
		for (int j = i + 1; j < 4; ++j) 
		{
			if (ApproxEqual(vertices[i].xyCoor, vertices[j].xyCoor)) 
			{
				// Two of the vertices are coincident, so we can eliminate at
				// least one triangle. We might be able to eliminate the other
				// as well, but this seems sufficient to avoid degenerate
				// triangulations.
				int indices[3] = { 0 };
				int index = 0;
				for (int k = 0; k < 4; ++k)
					if (k != j)
						indices[index++] = k;

				AddBezierTriangle(vertices[indices[0]], vertices[indices[1]], vertices[indices[2]], bzTriangles);

				return;
			}
		}
	}

	// See whether any of the points are fully contained in the
	// triangle defined by the other three.
	for (int i = 0; i < 4; ++i) 
	{
		int indices[3] = { 0 };
		int index = 0;
		for (int j = 0; j < 4; ++j)
			if (i != j)
				indices[index++] = j;

		if (PointInTriangle(vertices[i].xyCoor, vertices[indices[0]].xyCoor, vertices[indices[1]].xyCoor, vertices[indices[2]].xyCoor)) 
		{
			// Produce three triangles surrounding this interior vertex.
			for (int j = 0; j < 3; ++j)
				AddBezierTriangle(vertices[indices[j % 3]], vertices[indices[(j + 1) % 3]], vertices[i], bzTriangles);

			// Mark the interior vertex so we ignore it if trying to trace
			// the interior edge. What is this ???
			//m_vertices[i].setInterior(true);
			return;
		}
	}

	// There are only a few permutations of the vertices, ignoring
	// rotations, which are irrelevant:
	//
	//  0--3  0--2  0--3  0--1  0--2  0--1
	//  |  |  |  |  |  |  |  |  |  |  |  |
	//  |  |  |  |  |  |  |  |  |  |  |  |
	//  1--2  1--3  2--1  2--3  3--1  3--2
	//
	// Note that three of these are reflections of each other.
	// Therefore there are only three possible triangulations:
	//
	//  0--3  0--2  0--3
	//  |\ |  |\ |  |\ |
	//  | \|  | \|  | \|
	//  1--2  1--3  2--1
	//
	// From which we can choose by seeing which of the potential
	// diagonals intersect. Note that we choose the shortest diagonal
	// to split the quad.
	if (LinesIntersect(vertices[0].xyCoor, vertices[2].xyCoor, vertices[1].xyCoor, vertices[3].xyCoor)) 
	{
		if ((vertices[2].xyCoor - vertices[0].xyCoor).LengthSquared() < (vertices[3].xyCoor - vertices[1].xyCoor).LengthSquared()) 
		{
			AddBezierTriangle(vertices[0], vertices[1], vertices[2], bzTriangles);
			AddBezierTriangle(vertices[0], vertices[2], vertices[3], bzTriangles);
		}
		else 
		{
			AddBezierTriangle(vertices[0], vertices[1], vertices[3], bzTriangles);
			AddBezierTriangle(vertices[1], vertices[2], vertices[3], bzTriangles);
		}
	} 
	else if (LinesIntersect(vertices[0].xyCoor, vertices[3].xyCoor, vertices[1].xyCoor, vertices[2].xyCoor)) 
	{
		if ((vertices[3].xyCoor - vertices[0].xyCoor).LengthSquared() < (vertices[2].xyCoor - vertices[1].xyCoor).LengthSquared()) 
		{
			AddBezierTriangle(vertices[0], vertices[1], vertices[3], bzTriangles);
			AddBezierTriangle(vertices[0], vertices[3], vertices[2], bzTriangles);
		} 
		else 
		{
			AddBezierTriangle(vertices[0], vertices[1], vertices[2], bzTriangles);
			AddBezierTriangle(vertices[2], vertices[1], vertices[3], bzTriangles);
		}
	} 
	else 
	{
		// Lines (0->1), (2->3) intersect -- or should, modulo numerical
		// precision issues
		if ((vertices[1].xyCoor - vertices[0].xyCoor).LengthSquared() < (vertices[3].xyCoor - vertices[2].xyCoor).LengthSquared())
		{
			AddBezierTriangle(vertices[0],vertices[2], vertices[1], bzTriangles);
			AddBezierTriangle(vertices[0], vertices[1], vertices[3], bzTriangles);
		} 
		else 
		{
			AddBezierTriangle(vertices[0],vertices[2], vertices[3], bzTriangles);
			AddBezierTriangle(vertices[3], vertices[2],vertices[1], bzTriangles);
		}
	}
}

CurveType CubicBezierUtility::DetermineType (CVSystem::MyPoint    v0,
										  CVSystem::MyPoint    v1,
										  CVSystem::MyPoint    v2,
										  CVSystem::MyPoint    v3,
										  double& d0, double& d1,
										  double& d2, double& d3)
{
	d0 = 0;

	
	Eigen:: Vector3d b0(v0.x, v0.y, 1.0f);
	Eigen:: Vector3d b1(v1.x, v1.y, 1.0f);
	Eigen:: Vector3d b2(v2.x, v2.y, 1.0f);
	Eigen:: Vector3d b3(v3.x, v3.y, 1.0f);

	/*
	Eigen:: Vector3d b0(v0.x, v0.y, 0.0f);
	Eigen:: Vector3d b1(v1.x, v1.y, 0.0f);
	Eigen:: Vector3d b2(v2.x, v2.y, 0.0f);
	Eigen:: Vector3d b3(v3.x, v3.y, 0.0f);*/

	double a1 = b0.dot(b3.cross(b2));
	double a2 = b1.dot(b0.cross(b3));
	double a3 = b2.dot(b1.cross(b0));

	d1 = a1 - 2 * a2 + 3 * a3;
	d2 = -a2 + 3 * a3;
	d3 = 3 * a3;

	// Normalize
	double length = sqrt(d0 * d0 + d1 * d1 + d2* d2 + d3 * d3);
	d0 /= length;
	d1 /= length;
	d2 /= length;
	d3 /= length;

	double D = 3.0 * d2 * d2 - 4.0 * d1 * d3;
	double disc = d1 * d1 * D;

	// Rounding
	if (disc > -M_EPS && disc < M_EPS) disc = 0;
	if (d0 > -M_EPS && d0 < M_EPS) d0 = 0;
	if (d1 > -M_EPS && d1 < M_EPS) d1 = 0;
	if (d2 > -M_EPS && d2 < M_EPS) d2 = 0;
	if (d3 > -M_EPS && d3 < M_EPS) d3 = 0;

	if (!disc) 
	{
		if (!d1 && !d2) 
		{
			if (!d3) { return CURVE_TYPE_LINE; }
			return CURVE_TYPE_QUADRATIC;
		}

		if (d1 != 0) { return CURVE_TYPE_CUSP; }

		if (D < 0) { return CURVE_TYPE_LOOP; }
		return CURVE_TYPE_SERPENTINE;
	}

	if (disc > 0)  { return CURVE_TYPE_SERPENTINE; }

	// discriminant < 0
	return CURVE_TYPE_LOOP;
}

void CubicBezierUtility::AddPlainTriangle(CVSystem::MyPoint p1, 
										  CVSystem::MyPoint p2, 
										  CVSystem::MyPoint p3,
										  std::vector<MyCurveTriangle>& piTriangles,
										  std::vector<MyCurveTriangle>& poTriangles)
{
	bool isInside = IsClockwise(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
	MyCurveTriangle tri(p1, p2, p3);

	tri.startIndex = CubicBezierUtility::startIndex;
	tri.endIndex   = CubicBezierUtility::endIndex;
	tri.startPoint = CubicBezierUtility::startPoint;
	tri.endPoint   = CubicBezierUtility::endPoint;

	if(isInside) { piTriangles.push_back(tri); }
	else { poTriangles.push_back(tri); }
}

void CubicBezierUtility::AddBezierTriangle(MyVertex v0, MyVertex v1, MyVertex v2, std::vector<MyCurveTriangle>& bzTriangles)
{
	MyCurveTriangle tri(v0.xyCoor, v1.xyCoor, v2.xyCoor, v0.k, v0.l, v0.m, v1.k, v1.l, v1.m, v2.k, v2.l, v2.m);

	tri.startIndex = CubicBezierUtility::startIndex;
	tri.endIndex   = CubicBezierUtility::endIndex;
	tri.startPoint = CubicBezierUtility::startPoint;
	tri.endPoint   = CubicBezierUtility::endPoint;

	bzTriangles.push_back(tri);
}

void CubicBezierUtility::DrawPlainTriangle (double x0, double y0,
										 double x1, double y1,
										 double x2, double y2, bool inside)
{
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
	double blackness = SystemParams::cr_blackness;
	if(inside) { glColor4f(1.0, 1.0, 1.0, 1.0); }
	else { glColor4f(blackness, blackness, blackness, 1.0); }

	//if(inside) glColor4f(1.0, 0.0, 0.0, 1.0);
	//else glColor4f(0.0, 1.0, 0.0, 1.0);

	glBegin (GL_TRIANGLES);
	glVertex3f (x0, y0, 0.0f);
	glVertex3f (x1, y1, 0.0f);
	glVertex3f (x2, y2, 0.0f);
	glEnd ();

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

void CubicBezierUtility::DrawBezierTriangle (double x0, double y0,
										  double x1, double y1,
										  double x2, double y2,
										  double st0, double st1, double st2,
										  double st3, double st4, double st5,
										  double st6, double st7, double st8)
{
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	
	glBegin (GL_TRIANGLES);
	glTexCoord3f (st0, st1, st2);
	glVertex3f (x0, y0, 0.0f);
	glTexCoord3f (st3, st4, st5);
	glVertex3f (x1, y1, 0.0f);
	glTexCoord3f (st6, st7, st8);
	glVertex3f (x2, y2, 0.0f);
	glEnd ();

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

bool CubicBezierUtility::ApproxEqual(CVSystem::MyPoint& v0, CVSystem::MyPoint& v1)
{
	return (v0 - v1).LengthSquared() < M_EPS * M_EPS;
}

bool CubicBezierUtility::PointInTriangle(CVSystem::MyPoint& point, CVSystem::MyPoint& a, CVSystem::MyPoint& b, CVSystem::MyPoint& c)
{
	// Algorithm from http://www.blackpawn.com/texts/pointinpoly/default.html
	double x0 = c.x - a.x;
	double y0 = c.y - a.y;
	double x1 = b.x - a.x;
	double y1 = b.y - a.y;
	double x2 = point.x - a.x;
	double y2 = point.y - a.y;

	double dot00 = x0 * x0 + y0 * y0;
	double dot01 = x0 * x1 + y0 * y1;
	double dot02 = x0 * x2 + y0 * y2;
	double dot11 = x1 * x1 + y1 * y1;
	double dot12 = x1 * x2 + y1 * y2;
	double denominator = dot00 * dot11 - dot01 * dot01;
	if (!denominator)
		// Triangle is zero-area. Treat query point as not being inside.
			return false;
	// Compute
	double inverseDenominator = 1.0f / denominator;
	double u = (dot11 * dot02 - dot01 * dot12) * inverseDenominator;
	double v = (dot00 * dot12 - dot01 * dot02) * inverseDenominator;

	return (u > 0.0f) && (v > 0.0f) && (u + v < 1.0f);
}

// Utility functions local to this file.
int CubicBezierUtility::Orientation(CVSystem::MyPoint& p1, CVSystem::MyPoint& p2, CVSystem::MyPoint& p3)
{
	double crossProduct = (p2.y - p1.y) * (p3.x - p2.x) - (p3.y - p2.y) * (p2.x - p1.x);
	return (crossProduct < 0.0f) ? -1 : ((crossProduct > 0.0f) ? 1 : 0);
}

bool CubicBezierUtility::LinesIntersect(CVSystem::MyPoint& p1, CVSystem::MyPoint& q1, CVSystem::MyPoint& p2, CVSystem::MyPoint& q2)
{
	return (Orientation(p1, q1, p2) != Orientation(p1, q1, q2) && Orientation(p2, q2, p1) != Orientation(p2, q2, q1));
}

bool CubicBezierUtility::IsClockwise (double x0, double y0, double x1, double y1, double x2, double y2)
{
	double edge0[2];
	double edge1[2];

	edge0[0] = x0 - x1;
	edge0[1] = y0 - y1;
	edge1[0] = x2 - x1;
	edge1[1] = y2 - y1;

	if ((edge0[0] * edge1[1] - edge0[1] * edge1[0]) > 0.0f) return FALSE;
	else return TRUE;

}

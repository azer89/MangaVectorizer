
/**
 * Triangle to be passed to fragment shader
 * for Resolution independent curve rendering (See blinn-loop paper)
 *
 * Loop, C. & Blinn, J. 
 * Resolution Independent Curve Rendering Using Programmable Graphics Hardware 
 * ACM Trans. Graph., ACM, 2005, 24, 1000-1009
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#ifndef __My_Curve_Triangle__
#define __My_Curve_Triangle__

#include "MyPoint.h"


struct MyCurveTriangle
{
public:

	// nasty thing here...
	// Todo: make these four variables inside function parameters
	int startIndex;
	int endIndex;
	CVSystem::MyPoint startPoint;
	CVSystem::MyPoint endPoint;

	CVSystem::MyPoint p0;
	CVSystem::MyPoint p1;
	CVSystem::MyPoint p2;

	double texCoords[9];
	
	// Constructor
	MyCurveTriangle(CVSystem::MyPoint p0, CVSystem::MyPoint p1, CVSystem::MyPoint p2, 
					double s0,	double s1,	double s2,
					double s3,	double s4,	double s5,
					double s6,	double s7,	double s8,
					int startIndex,
					int endIndex,
					CVSystem::MyPoint startPoint,
					CVSystem::MyPoint endPoint)
	{
		this->p0 = p0;
		this->p1 = p1;
		this->p2 = p2;

		this->texCoords[0] = s0;
		this->texCoords[1] = s1;
		this->texCoords[2] = s2;
		this->texCoords[3] = s3;
		this->texCoords[4] = s4;
		this->texCoords[5] = s5;
		this->texCoords[6] = s6;
		this->texCoords[7] = s7;
		this->texCoords[8] = s8;

		this->startIndex = startIndex;
		this->endIndex   = endIndex;
		this->startPoint = startPoint;
		this->endPoint   = endPoint;
	}


	// Constructor
	MyCurveTriangle(CVSystem::MyPoint p0, CVSystem::MyPoint p1, CVSystem::MyPoint p2, 
					double s0,	double s1,	double s2,
					double s3,	double s4,	double s5,
					double s6,	double s7,	double s8)
	{
		this->p0 = p0;
		this->p1 = p1;
		this->p2 = p2;

		this->texCoords[0] = s0;
		this->texCoords[1] = s1;
		this->texCoords[2] = s2;
		this->texCoords[3] = s3;
		this->texCoords[4] = s4;
		this->texCoords[5] = s5;
		this->texCoords[6] = s6;
		this->texCoords[7] = s7;
		this->texCoords[8] = s8;

		this->startIndex = -1;
		this->endIndex   = -1;
		this->startPoint = CVSystem::MyPoint();
		this->endPoint   = CVSystem::MyPoint();
	}

	// Consttructor
	MyCurveTriangle(CVSystem::MyPoint p0, CVSystem::MyPoint p1, CVSystem::MyPoint p2)
	{
		this->p0 = p0;
		this->p1 = p1;
		this->p2 = p2;

		this->texCoords[0] = 
		this->texCoords[1] = 
		this->texCoords[2] = 
		this->texCoords[3] = 
		this->texCoords[4] = 
		this->texCoords[5] = 
		this->texCoords[6] = 
		this->texCoords[7] = 
		this->texCoords[8] = -1;

		this->startIndex = -1;
		this->endIndex   = -1;
		this->startPoint = CVSystem::MyPoint();
		this->endPoint   = CVSystem::MyPoint();
	}

	// 
	bool ArePointsValid(std::vector<CVSystem::MyPoint> vertexList)
	{
		CVSystem::MyPoint realStartPoint = vertexList[startIndex];
		CVSystem::MyPoint realEndPoint = vertexList[endIndex];

		if(realStartPoint.Distance(startPoint) < M_EPS && realEndPoint.Distance(endPoint) < M_EPS )
			{ return true; }

		return false;
	}
};

#endif
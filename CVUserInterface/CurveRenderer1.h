
/**
 * Early version of bezier curve rendering,
 * you don't need to use this class
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __Curve_Renderer_V1__
#define __Curve_Renderer_V1__


#include <vector>

#include "CurveType.h"
#include "CubicBezierUtility.h"

class CurveRenderer1
{
public:
	// Constructor
	CurveRenderer1();

	// Destructor
	~CurveRenderer1();

	// set quads data
	void SetQuads(std::vector<CVSystem::MyQuad> quads);

	// get quads data
	std::vector<CVSystem::MyQuad> GetQuads() { return inputQuads; }
	
	// init instance
	void Init();

	// draw
	void paintGL();

	// reset everything
	void Reset()
	{
		this->inputQuads.clear();
		this->bzTriangles.clear();
		this->piTriangles.clear();
		this->poTriangles.clear();
	}

private:

	std::vector<CVSystem::MyQuad> inputQuads;
	std::vector<MyCurveTriangle> bzTriangles;	// shader

	std::vector<MyCurveTriangle> piTriangles;	// plain inside
	std::vector<MyCurveTriangle> poTriangles;	// plain outside

	QGLShaderProgram shaderProgram;
	
};

#endif
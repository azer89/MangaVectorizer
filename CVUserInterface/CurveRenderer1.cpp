
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "CurveRenderer1.h"
#include "SystemParams.h"

CurveRenderer1::CurveRenderer1() /*: polyFlag(0)*/
{
}

CurveRenderer1::~CurveRenderer1()
{
}

// Should be called during initializeGL()
void CurveRenderer1::Init()
{
	//this->canRender = false;
	this->Reset();

	shaderProgram.addShaderFromSourceFile(QGLShader::Fragment,	"CubicBezier.fsh");
	shaderProgram.link();
}

void CurveRenderer1::SetQuads(std::vector<CVSystem::MyQuad> quads)
{
	this->inputQuads = quads;
	this->bzTriangles.clear();
	for(size_t a = 0; a < inputQuads.size(); a++)
	{
		CVSystem::MyQuad q = inputQuads[a];		
		//this->curIdx = inputQuads[a].polyIdx;		
		CubicBezierUtility::ComputeCubic(q.p0.x, q.p0.y, q.p1.x, q.p1.y, q.p2.x, q.p2.y, q.p3.x, q.p3.y, bzTriangles, piTriangles, poTriangles);
	}
}

void CurveRenderer1::paintGL()
{
	shaderProgram.bind();

	int blackness = SystemParams::cr_blackness;
	shaderProgram.setUniformValue("insideColor",  QColor(Qt::white));
	shaderProgram.setUniformValue("outsideColor", QColor(blackness, blackness, blackness, 255));

	for(size_t a = 0; a < bzTriangles.size(); a++)
	{
		MyCurveTriangle ct = bzTriangles[a];
		CubicBezierUtility::DrawBezierTriangle(ct.p0.x, ct.p0.y, ct.p1.x, ct.p1.y, ct.p2.x, ct.p2.y, 
											   ct.texCoords[0], ct.texCoords[1], ct.texCoords[2],
											   ct.texCoords[3], ct.texCoords[4], ct.texCoords[5],
											   ct.texCoords[6], ct.texCoords[7], ct.texCoords[8]);
	}

	shaderProgram.release();

	// inside
	for(size_t a = 0; a < piTriangles.size(); a++)
	{
		MyCurveTriangle ct = piTriangles[a];
		CubicBezierUtility::DrawPlainTriangle(ct.p0.x, ct.p0.y, ct.p1.x, ct.p1.y, ct.p2.x, ct.p2.y, true);
	}

	// outside
	for(size_t a = 0; a < poTriangles.size(); a++)
	{
		MyCurveTriangle ct = poTriangles[a];
		CubicBezierUtility::DrawPlainTriangle(ct.p0.x, ct.p0.y, ct.p1.x, ct.p1.y, ct.p2.x, ct.p2.y, false);
	}
}

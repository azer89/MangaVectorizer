
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "CurveType.h"
#include "MyVertex.h"
#include "CurveRenderer2.h"
#include "SystemParams.h"
#include "CubicBezierUtility.h"

CurveRenderer2::CurveRenderer2()
{
}

CurveRenderer2::~CurveRenderer2()
{
}

// Should be called during initializeGL()
void CurveRenderer2::Init()
{
	cubicBezierShaderProgram.addShaderFromSourceFile(QGLShader::Fragment,	"CubicBezier.fsh");
	cubicBezierShaderProgram.link();
}

void CurveRenderer2::paintGL()
{
	RenderBezierCurves();
}

QGLFramebufferObject* CurveRenderer2::RenderCurvesToFBO(int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor)
{
	QGLFramebufferObject* frameBufferA = new QGLFramebufferObject(frame_width, frame_height, GL_TEXTURE_2D );
	
	frameBufferA->bind();
	RenderBezierCurves();
	frameBufferA->release();
	
	return frameBufferA;
}

void CurveRenderer2::RenderBezierCurves()
{
	cubicBezierShaderProgram.bind();

	int blackness = (int)(SystemParams::cr_blackness * 255.0);
	cubicBezierShaderProgram.setUniformValue("insideColor",  QColor(Qt::white));
	cubicBezierShaderProgram.setUniformValue("outsideColor", QColor(blackness, blackness, blackness, 255));

	for(size_t a = 0; a < bzTriangles.size(); a++)
	{
		MyCurveTriangle ct = bzTriangles[a];
		CubicBezierUtility::DrawBezierTriangle(ct.p0.x, ct.p0.y, ct.p1.x, ct.p1.y, ct.p2.x, ct.p2.y, 
												ct.texCoords[0], ct.texCoords[1], ct.texCoords[2],
												ct.texCoords[3], ct.texCoords[4], ct.texCoords[5],
												ct.texCoords[6], ct.texCoords[7], ct.texCoords[8]);
	}

	cubicBezierShaderProgram.release();

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


void CurveRenderer2::Recalculate(std::vector<CVSystem::MyPoint> vertexList)
{
	this->bzTriangles.clear();
	this->piTriangles.clear();
	this->poTriangles.clear();
	this->SetBezierCurves(this->inputCurves, vertexList);
}

void CurveRenderer2::SetBezierCurves(std::vector<CVSystem::MyIndexedBezierCurves> indexedBezierCurves, std::vector<CVSystem::MyPoint> vertexList)
{
	using namespace CVSystem;

	this->inputCurves = indexedBezierCurves;

	for(size_t a = 0; a < inputCurves.size(); a++)
	{
		MyIndexedBezierCurves* curves = &inputCurves[a];
		
		curves->p0_points.resize(curves->indices.size());
		curves->p1_points.resize(curves->indices.size());
		curves->p2_points.resize(curves->indices.size());

		for(size_t b = 0; b < curves->indices.size() - 1; b++)
		{
			// four control points
			MyPoint p0; MyPoint p1; MyPoint p2; MyPoint p3;

			// before p0, after p3
			MyPoint pPrev; MyPoint pNext;

			p0 = vertexList[curves->indices[b]];
			p3 = vertexList[curves->indices[b+1]];

			curves->p0_points[b] = p0;

			if( b == 0)
			{
				pPrev = vertexList[curves->indices[curves->indices.size() - 2]];
				pNext = vertexList[curves->indices[b+2]];
			}
			else
			{
				pPrev = vertexList[curves->indices[b-1]];
				if(b == curves->indices.size() - 2) { pNext = vertexList[curves->indices[0]]; }
				else { pNext = vertexList[curves->indices[b+2]]; }
			}

			CurveToBezier(pPrev, p0, p3, pNext, p1, p2);

			curves->p1_points[b] = p1;
			curves->p2_points[b] = p2;

			// Todo: remove this
			CubicBezierUtility::startPoint = p0;
			CubicBezierUtility::endPoint   = p3;
			CubicBezierUtility::startIndex = curves->indices[b];
			CubicBezierUtility::endIndex   = curves->indices[b+1];

			CubicBezierUtility::ComputeCubic(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, bzTriangles, piTriangles, poTriangles);

		}
	}
}

void CurveRenderer2::CurveToBezier(CVSystem::MyPoint p0, CVSystem::MyPoint p1, CVSystem::MyPoint p2, CVSystem::MyPoint p3, CVSystem::MyPoint& cp0, CVSystem::MyPoint& cp1)
{
	double xc1 = (p0.x + p1.x) / 2.0;		double yc1 = (p0.y + p1.y) / 2.0;
	double xc2 = (p1.x + p2.x) / 2.0;		double yc2 = (p1.y + p2.y) / 2.0;
	double xc3 = (p2.x + p3.x) / 2.0;		double yc3 = (p2.y + p3.y) / 2.0;

	double len1 = sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
	double len2 = sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
	double len3 = sqrt((p3.x - p2.x) * (p3.x - p2.x) + (p3.y - p2.y) * (p3.y - p2.y));

	double k1 = len1 / (len1 + len2);		double k2 = len2 / (len2 + len3);

	double xm1 = xc1 + (xc2 - xc1) * k1;	double ym1 = yc1 + (yc2 - yc1) * k1;
	double xm2 = xc2 + (xc3 - xc2) * k2;	double ym2 = yc2 + (yc3 - yc2) * k2;

	// Resulting control points. Here smooth_value is mentioned
	// above coefficient K whose value should be in range [0...1].
	cp0.x = xm1 + (xc2 - xm1) * SystemParams::t_smooth_factor + p1.x - xm1;
	cp0.y = ym1 + (yc2 - ym1) * SystemParams::t_smooth_factor + p1.y - ym1;

	cp1.x = xm2 + (xc2 - xm2) * SystemParams::t_smooth_factor + p2.x - xm2;
	cp1.y = ym2 + (yc2 - ym2) * SystemParams::t_smooth_factor + p2.y - ym2;
}


/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "ColorCurveRenderer.h"

ColorCurveRenderer::ColorCurveRenderer()
{
}

ColorCurveRenderer::~ColorCurveRenderer()
{
}

void ColorCurveRenderer::paintGL()
{
	using namespace CVSystem;

	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(1.0);
	glBegin(GL_LINES);
	
	for(size_t a = 0; a < _indexedBezierCurves.size(); a++)
	{
		//QColor col(_colors[a]);
		//glColor4f(col.red() / 255.0, col.green() / 255.0, col.blue() / 255.0, 1.0);
		MyIndexedBezierCurves curve = _indexedBezierCurves[a];

		//if(curve.isClockwise) { glColor4f(0, 0, 0, 1.0);  }
		//else { glColor4f(0.75, 0.75, 0.75, 1.0); }
		glColor4f(0, 0, 0, 1.0);

		for(size_t b = 0; b < curve.indices.size() - 1; b++)
		{
			MyPoint pA = _vertexList[curve.indices[b]]; MyPoint pB = _vertexList[curve.indices[b+1]];
			glVertex3f(pA.x, pA.y, 0);	 glVertex3f(pB.x, pB.y, 0);
		}
	}
	glEnd();

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

void ColorCurveRenderer::SetData(std::vector<CVSystem::MyPoint> vertexList, std::vector<CVSystem::MyIndexedBezierCurves> indexedBezierCurves)
{
	//// Assign to vectors
	this->_vertexList		   = vertexList;
	this->_indexedBezierCurves = indexedBezierCurves;

	for(size_t a = 0; a < _indexedBezierCurves.size(); a++)
		{ _indexedBezierCurves[a].CalculateOrientation(_vertexList); }

	//for(size_t a = 0; a < _indexedBezierCurves.size(); a++)
	//	{ _colors.push_back(qRgb(rand() % 255, rand() % 255, rand() % 255)); }
}

/**
 * Class to render the curve
 * the pros for this class compared to CurveRenderer1, is this class uses Framebuffer Object
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __Curve_Renderer_V2__
#define __Curve_Renderer_V2__

#include <vector>

class CurveRenderer2
{
public:
	// Constructor
	CurveRenderer2();

	// Destructor
	~CurveRenderer2();

	// Initialize instance
	void Init();

	// Draw
	void paintGL();
	
	// Render bezier curve to framebuffer object
	QGLFramebufferObject* RenderCurvesToFBO(int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor);
	
	// set data: bezier curve and vertex list
	void SetBezierCurves(std::vector<CVSystem::MyIndexedBezierCurves> indexedBezierCurves, std::vector<CVSystem::MyPoint> vertexList);

	// update bezier curve after deformation
	void Recalculate(std::vector<CVSystem::MyPoint> vertexList);

private:
	std::vector<CVSystem::MyIndexedBezierCurves> inputCurves;

	std::vector<MyCurveTriangle> bzTriangles;	// shader
	std::vector<MyCurveTriangle> piTriangles;	// plain inside
	std::vector<MyCurveTriangle> poTriangles;	// plain outside
	
	QGLShaderProgram cubicBezierShaderProgram;

private:
	void RenderBezierCurves();

	void CurveToBezier(CVSystem::MyPoint p0, CVSystem::MyPoint p1, CVSystem::MyPoint p2, CVSystem::MyPoint p3, CVSystem::MyPoint& cp0, CVSystem::MyPoint& cp1);
};

#endif
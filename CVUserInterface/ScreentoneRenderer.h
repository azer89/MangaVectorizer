
/**
 * Render screentone pattern using procedural shaders
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __Screentone_Renderer_V2__
#define __Screentone_Renderer_V2__

#include <vector>
#include <QtOpenGL/QGLWidget>
#include <QGLShader>
#include <QGLFramebufferObject>

#include "MyPoint.h"
#include "MyIndexedTriangle.h"
#include "TriangleType.h"

#include "UtilityFunctions.h"
#include "SystemParams.h"

class ScreentoneRenderer
{
public:
	// Constructor
	ScreentoneRenderer();

	// Destructor
	~ScreentoneRenderer();

	// set orientation of screentone pattern (manually by users)
	void SetOrientation(double degAngle)
		{ if(params.size() >= 2) { params[1] = degAngle; } }

	// set image size
	void SetVariables(double img_width, double img_height);

	// set properties
	void SetParams(std::vector<double> params);

	// set triangle data
	void SetTriangles(std::vector<CVSystem::MyPoint> vertexList, std::vector<CVSystem::MyIndexedTriangle> scTriangles, std::vector<CVSystem::MyIndexedTriangle> borderSCTriangles);
	// update vertices after deformation
	void UpdateVertices(std::vector<CVSystem::MyPoint> vertexList) { this->_vertexList = vertexList; }

	//// rendering without AA (faster FPS, worse result)
	// No lighting
	QGLFramebufferObject* RenderToFBO(int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor);
	// Lighting
	QGLFramebufferObject* RenderToFBOWithLighting(GLuint gradationImgID, double thicknessFactor, int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor);

	//// rendering with AA (slower FPS, better result)
	// No Lighting
	QGLFramebufferObject* RenderToFBOAA(int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor);
	// Lighting
	QGLFramebufferObject* RenderToFBOWithLightingAA(GLuint gradationImgID, double thicknessFactor, int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor);
	
private:
	// draw
	void DrawScreentone(double scaleFactor);
	// draw with lighting e8ffect
	void DrawScreentoneWithLighting(double scaleFactor);

	// render to framebuffer object with blur shader
	void RenderFBOWithBlur(int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor, QGLFramebufferObject* frameBuffer, double blurFactor, bool xAxis = true);

private:
	// shader without lighting
	QGLShaderProgram scShaderProgram1;
	// shader with lighting
	QGLShaderProgram scShaderProgram2;
	// blur shader 
	QGLShaderProgram blurShaderProgram;

	// screentone properties (orientation, spacing, radius/thickness/side)
	std::vector<double> params;

	// vertices
	std::vector<CVSystem::MyPoint>			  _vertexList;
	// indexed triangle mesh
	std::vector<CVSystem::MyIndexedTriangle>  _triangles;

	// current image size (width and height) multiplied with _zoomFactor
	double _img_width;
	double _img_height;

	// display zooming (mouse scroll)
	double _zoomFactor;

	// increase fbo to remove moire effect
	double _frameScale;

	// original image size
	int _ori_frame_width;
	int _ori_frame_height;
	int _ori_x_offset;
	int _ori_y_offset;

	// gradation mask for lighting effect
	GLuint _gradationImgID;
	// users can change the strength of the light
	double _thicknessFactor;

	// scroll offsets
	double offset_x;
	double offset_y;
};

#endif

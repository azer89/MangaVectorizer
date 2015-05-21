
/**
 * Class contains some drawing functions:
 *		- Draw wireframe
 *		- Draw triangles
 *		- Draw circles
 *		- Draw lines
 *		- Framebuffer Object
 *		- Anti-aliasing with FXAA
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __Mesh_Renderer__
#define __Mesh_Renderer__

#include "stdafx.h"

class CurveRenderer1;
class CurveRenderer2;

class RenderUtility
{
private:
	// white artifact removal shader
	QGLShaderProgram whiteArtifactShaderProgram;
	// blur shader
	QGLShaderProgram blurShaderProgram;
	// FXAA shader
	QGLShaderProgram fxaaShaderProgram;

public:
	// Constructor
	RenderUtility();

	// Destructor
	~RenderUtility();

	// draw indexed triangle mesh
	void DrawTriangles(std::vector<CVSystem::MyPoint> vertexList, std::vector<CVSystem::MyIndexedTriangle>  iTriangles, double r, double g, double b);
	// draw non-indexed triangle mesh
	void DrawTriangles(std::vector<CVSystem::MyTriangle>  triangles, double r, double g, double b);
	// draw wireframe of indexed triangle mesh
	void DrawWireframes(std::vector<CVSystem::MyPoint> vertexList,  std::vector<CVSystem::MyIndexedTriangle>  iTriangles, double r, double g, double b);
	// draw wireframe of non-indexed triangle mesh
	void DrawWireframes(std::vector<CVSystem::MyTriangle> triangles, double r, double g, double b);

	// draw a colored circle
	void DrawCircle(GLfloat x, GLfloat y, GLfloat radius);

	// draw strokes
	void DrawStrokeLines(std::vector<std::vector<CVSystem::MyLine>> uStrokeLines, std::vector<QRgb> uStrokeColors);

	// draw white rectangle
	void DrawWhiteRectangle(int width, int height);
	
	// shader to remove artifact while rendering bezier curve
	QGLFramebufferObject* RemoveWhitePixels(int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor, QGLFramebufferObject* frameBuffer)
	{
		QGLFramebufferObject* retFrameBuffer = new QGLFramebufferObject(frame_width, frame_height);

		glEnable(GL_TEXTURE_2D);

		double xLeft = 0 + x_offset;
		double xRight = frame_width + x_offset;

		double yTop = 0 + y_offset;
		double yBottom = frame_height + y_offset;

		double invScale = 1.0 / zoom_factor;
		xLeft   *= invScale;
		xRight  *= invScale;
		yTop    *= invScale;
		yBottom *= invScale;

		retFrameBuffer->bind();
		whiteArtifactShaderProgram.bind();

		glDepthMask(GL_FALSE);	
		glEnable(GL_BLEND);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(1.0, 1.0, 1.0, 1.0);

		glBindTexture( GL_TEXTURE_2D, frameBuffer->texture() );

		whiteArtifactShaderProgram.setUniformValue("tex", frameBuffer->texture());

		glBegin(GL_QUADS);

		// text coords are flipped in y axis
		glTexCoord2d(0,1); glVertex3d(xLeft,  yTop,    0);
		glTexCoord2d(1,1); glVertex3d(xRight, yTop,    0);
		glTexCoord2d(1,0); glVertex3d(xRight, yBottom, 0);
		glTexCoord2d(0,0); glVertex3d(xLeft,  yBottom, 0);

		glEnd();

		glFlush();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);

		whiteArtifactShaderProgram.release();
		retFrameBuffer->release();

		return retFrameBuffer;
	}
	
	// draw everything
	void DrawFullscreen(QGLFramebufferObject* curveFBO, 
					    QGLFramebufferObject* scFBO,
						std::vector<CVSystem::MyPoint>			  _vertexList,
						std::vector<CVSystem::MyIndexedTriangle>  _borderWTriangles,
						std::vector<CVSystem::MyIndexedTriangle>  _wTriangles,
						std::vector<CVSystem::MyIndexedTriangle>  _bTriangles,
						int x_offset,
						int y_offset,
						int frame_width,
						int frame_height,
						double zoom_factor);

	// Draw bezier curve
	template <class T>
	void DrawBezierCurves(std::vector<CVSystem::MyPoint> vertexList, 
						  std::vector<CVSystem::MyIndexedTriangle>  bTriangles,
						  T curveRenderer,
						  int x_offset,
						  int y_offset,
						  int frame_width,
						  int frame_height,
						  double zoom_factor)
	{
		QGLFramebufferObject frameBuffer(frame_width, frame_height);
		frameBuffer.bind();

		double blackness = SystemParams::cr_blackness;
		DrawTriangles(vertexList, bTriangles, blackness, blackness, blackness);
		if(curveRenderer) { curveRenderer->paintGL(); }

		frameBuffer.release();

		glEnable(GL_TEXTURE_2D);

		double xLeft = 0 + x_offset;
		double xRight = frame_width + x_offset;

		double yTop = 0 + y_offset;
		double yBottom = frame_height + y_offset;

		double invScale = 1.0 / zoom_factor;
		xLeft   *= invScale;
		xRight  *= invScale;
		yTop    *= invScale;
		yBottom *= invScale;

		QGLFramebufferObject frameBufferA(frame_width, frame_height);
		QGLFramebufferObject frameBufferB(frame_width, frame_height);
		frameBufferA.bind();

		whiteArtifactShaderProgram.bind();

		glDepthMask(GL_FALSE);	
		glEnable(GL_BLEND);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(1.0, 1.0, 1.0, 1.0);

		glBindTexture( GL_TEXTURE_2D, frameBuffer.texture() );

		whiteArtifactShaderProgram.setUniformValue("tex", frameBuffer.texture());

		glBegin(GL_QUADS);

		// text coords are flipped in y axis
		glTexCoord2d(0,1); glVertex3d(xLeft,  yTop,    0);
		glTexCoord2d(1,1); glVertex3d(xRight, yTop,    0);
		glTexCoord2d(1,0); glVertex3d(xRight, yBottom, 0);
		glTexCoord2d(0,0); glVertex3d(xLeft,  yBottom, 0);

		glEnd();

		glFlush();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);

		whiteArtifactShaderProgram.release();

		frameBufferA.release();
		frameBufferB.bind();
		RenderFBOWithBlur(x_offset, y_offset, frame_width, frame_height, zoom_factor, &frameBufferA, SystemParams::cr_blur, true);
		frameBufferB.release();
		RenderFBOWithBlur(x_offset, y_offset, frame_width, frame_height, zoom_factor, &frameBufferB, SystemParams::cr_blur, false);
	}

	// draw framebuffer object
	void RenderFBO(int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor, QGLFramebufferObject* frameBuffer);
	// draw framebuffer object with blur shader
	void RenderFBOWithBlur(int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor, QGLFramebufferObject* frameBuffer, double blurFactor, bool xAxis);
	// draw framebuffer object with FXAA
	void RenderFBOWithFXAA(int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor, QGLFramebufferObject* frameBuffer);


	// draw simple white triangle
	void DrawWhiteRectangle(int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor);


	// draw texture (bitmap)
	void DrawTexture(GLuint texId, double width, double height);
};

#endif
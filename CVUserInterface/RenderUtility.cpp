
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "RenderUtility.h"
#include "UtilityFunctions.h"
#include "SystemParams.h"

#include "CurveRenderer1.h"
#include "CurveRenderer2.h"

RenderUtility::RenderUtility()
{
	whiteArtifactShaderProgram.addShaderFromSourceFile(QGLShader::Fragment, "NoWhitePixels.fsh");
	whiteArtifactShaderProgram.link();

	blurShaderProgram.addShaderFromSourceFile(QGLShader::Fragment, "GaussianBlur.fsh");
	blurShaderProgram.link();
		
	fxaaShaderProgram.addShaderFromSourceFile(QGLShader::Fragment, "FXAA.fsh");
	fxaaShaderProgram.link();
}

RenderUtility::~RenderUtility()
{
}

void RenderUtility::DrawFullscreen(QGLFramebufferObject* curveFBO, 
					QGLFramebufferObject* scFBO,
					std::vector<CVSystem::MyPoint>			  _vertexList,
					std::vector<CVSystem::MyIndexedTriangle>  _borderWTriangles,
					std::vector<CVSystem::MyIndexedTriangle>  _wTriangles,
					std::vector<CVSystem::MyIndexedTriangle>  _bTriangles,
					int x_offset,
					int y_offset,
					int frame_width,
					int frame_height,
					double zoom_factor)
{
	QGLFramebufferObject frameBufferA(frame_width, frame_height);
	QGLFramebufferObject frameBufferB(frame_width, frame_height);
	//QGLFramebufferObject frameBufferC(frame_width, frame_height);
	//QGLFramebufferObject frameBufferD(frame_width, frame_height);

	// FIRST PASS
	frameBufferA.bind();
	double blackness = SystemParams::cr_blackness;
	DrawTriangles(_vertexList, _bTriangles, blackness, blackness, blackness);
	if(curveFBO) { RenderFBO(x_offset, y_offset, frame_width, frame_height, zoom_factor, curveFBO); }
	frameBufferA.release();
	QGLFramebufferObject* freeWhiteBuffer = RemoveWhitePixels(x_offset, y_offset, frame_width, frame_height, zoom_factor, &frameBufferA);
	
	if(freeWhiteBuffer) { RenderFBO(x_offset, y_offset, frame_width, frame_height, zoom_factor, freeWhiteBuffer); }

	// SECOND PASS
	frameBufferB.bind();

	DrawWhiteRectangle(x_offset, y_offset, frame_width, frame_height, zoom_factor);

	DrawTriangles(_vertexList, _wTriangles, 1.0, 1.0, 1.0);
	DrawTriangles(_vertexList, _borderWTriangles, 1.0, 1.0, 1.0);
	if(scFBO) { RenderFBO(x_offset, y_offset, frame_width, frame_height, zoom_factor, scFBO); }
	if(freeWhiteBuffer) { RenderFBO(x_offset, y_offset, frame_width, frame_height, zoom_factor, freeWhiteBuffer); }
	frameBufferB.release();

	RenderFBOWithFXAA(x_offset, y_offset, frame_width, frame_height, zoom_factor, &frameBufferB);

	if(freeWhiteBuffer) {delete freeWhiteBuffer;}
}

void RenderUtility::DrawWhiteRectangle(int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor)
{
	//glEnable(GL_TEXTURE_2D);

	double xLeft = 0 + x_offset;
	double xRight = frame_width + x_offset;

	double yTop = 0 + y_offset;
	double yBottom = frame_height + y_offset;

	double invScale = 1.0 / zoom_factor;
	xLeft   *= invScale;
	xRight  *= invScale;
	yTop    *= invScale;
	yBottom *= invScale;

	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	//glBindTexture( GL_TEXTURE_2D, frameBuffer->texture() );

	glBegin(GL_QUADS);

	// text coords are flipped in y axis
	glVertex3d(xLeft,  yTop,    0);
	glVertex3d(xRight, yTop,    0);
	glVertex3d(xRight, yBottom, 0);
	glVertex3d(xLeft,  yBottom, 0);

	glEnd();

	glFlush();
	//glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

void RenderUtility::RenderFBO(int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor, QGLFramebufferObject* frameBuffer)
{
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

	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	glBindTexture( GL_TEXTURE_2D, frameBuffer->texture() );

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
}

void RenderUtility::RenderFBOWithFXAA(int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor, QGLFramebufferObject* frameBuffer)
{
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

	fxaaShaderProgram.bind();

	fxaaShaderProgram.setUniformValue("bgl_RenderedTextureWidth", (GLfloat)frame_width); 
	fxaaShaderProgram.setUniformValue("bgl_RenderedTextureHeight", (GLfloat)frame_height); 

	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	glBindTexture( GL_TEXTURE_2D, frameBuffer->texture() );

	fxaaShaderProgram.setUniformValue("tex", frameBuffer->texture());

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

	fxaaShaderProgram.release();
}

void RenderUtility::RenderFBOWithBlur(int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor, QGLFramebufferObject* frameBuffer, double blurFactor, bool xAxis)
{
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

	blurShaderProgram.bind();

	blurShaderProgram.setUniformValue("radius", (GLfloat)(blurFactor / zoom_factor) );

	if(xAxis) 
	{ 
		blurShaderProgram.setUniformValue("resolutionx", (GLfloat)frame_width); 
		blurShaderProgram.setUniformValue("dirx", (GLfloat)1.0); 
		blurShaderProgram.setUniformValue("diry", (GLfloat)0.0); 
	}
	else 
	{ 
		blurShaderProgram.setUniformValue("resolutiony", (GLfloat)frame_height);
		blurShaderProgram.setUniformValue("dirx", (GLfloat)0.0); 
		blurShaderProgram.setUniformValue("diry", (GLfloat)1.0); 
	}

	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	glBindTexture( GL_TEXTURE_2D, frameBuffer->texture() );

	blurShaderProgram.setUniformValue("tex", frameBuffer->texture());

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

	blurShaderProgram.release();
}

void RenderUtility::DrawTexture(GLuint texId, double width, double height)
{
	glEnable(GL_TEXTURE_2D);

	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	glBindTexture( GL_TEXTURE_2D, texId );

	glBegin(GL_QUADS);

	// text coords are flipped in y axis
	glTexCoord2d(0,1); glVertex3d(0,	 0,          0);
	glTexCoord2d(1,1); glVertex3d(width, 0,          0);
	glTexCoord2d(1,0); glVertex3d(width, height, 0);
	glTexCoord2d(0,0); glVertex3d(0,	 height, 0);

	glEnd();

	glFlush();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);  
}

void RenderUtility::DrawTriangles(std::vector<CVSystem::MyPoint> vertexList, 
								  std::vector<CVSystem::MyIndexedTriangle>  iTriangles,
								  double r, double g, double b)
{
	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(r, g, b, 1.0);
	glBegin(GL_TRIANGLES);

	for(size_t a = 0; a < iTriangles.size(); a++)
	{
		CVSystem::MyPoint p0 = vertexList[iTriangles[a].idx0];
		CVSystem::MyPoint p1 = vertexList[iTriangles[a].idx1];
		CVSystem::MyPoint p2 = vertexList[iTriangles[a].idx2];

		glVertex3f(p0.x, p0.y, 0);
		glVertex3f(p1.x, p1.y, 0);
		glVertex3f(p2.x, p2.y, 0);
	}

	glEnd();

	glFlush();
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE); 
}

void RenderUtility::DrawTriangles(std::vector<CVSystem::MyTriangle>  triangles,
				                 double r, double g, double b)
{
	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(r, g, b, 1.0);
	glBegin(GL_TRIANGLES);

	for(size_t a = 0; a < triangles.size(); a++)
	{
		CVSystem::MyPoint p0 = triangles[a].p1;
		CVSystem::MyPoint p1 = triangles[a].p2;
		CVSystem::MyPoint p2 = triangles[a].p3;

		glVertex3f(p0.x, p0.y, 0);
		glVertex3f(p1.x, p1.y, 0);
		glVertex3f(p2.x, p2.y, 0);
	}
	glEnd();

	glFlush();
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);  
}

void RenderUtility::DrawWireframes(std::vector<CVSystem::MyPoint> vertexList, 
								   std::vector<CVSystem::MyIndexedTriangle> iTriangles,
								   double r, double g, double b)
{
	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);	

	glColor4f(r, g, b, 0.5);
	glLineWidth(1.0);
	glBegin(GL_LINES);

	for(int a = 0; a < iTriangles.size(); a++)
	{
		CVSystem::MyPoint p0 = vertexList[iTriangles[a].idx0];
		CVSystem::MyPoint p1 = vertexList[iTriangles[a].idx1];
		CVSystem::MyPoint p2 = vertexList[iTriangles[a].idx2];

		glVertex3f(p0.x, p0.y, 0);	glVertex3f(p1.x, p1.y, 0);
		glVertex3f(p1.x, p1.y, 0);  glVertex3f(p2.x, p2.y, 0);
		glVertex3f(p0.x, p0.y, 0);  glVertex3f(p2.x, p2.y, 0);
	}
	glEnd();

	
	glFlush();
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);  
}

void RenderUtility::DrawWireframes(std::vector<CVSystem::MyTriangle> triangles,
								  double r, double g, double b)
{
	glDepthMask(GL_FALSE);	
	glLineWidth(1.0); 
	glColor4f(r, g, b, 1.0);
	glLineWidth(1.0);
	glBegin(GL_LINES);

	for(size_t a = 0; a < triangles.size(); a++)
	{
		CVSystem::MyPoint p0 = triangles[a].p1;
		CVSystem::MyPoint p1 = triangles[a].p2;
		CVSystem::MyPoint p2 = triangles[a].p3;

		glVertex3f(p0.x, p0.y, 0);	glVertex3f(p1.x, p1.y, 0);
		glVertex3f(p1.x, p1.y, 0);  glVertex3f(p2.x, p2.y, 0);
		glVertex3f(p0.x, p0.y, 0);  glVertex3f(p2.x, p2.y, 0);
	}
	glEnd();
	glDepthMask(GL_TRUE); 
}


void RenderUtility::DrawCircle(GLfloat x, GLfloat y, GLfloat radius)
{
	int i;
	int triangleAmount = 10;
	GLfloat twicePi = 2.0f * M_PI;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y); // center of circle
	for(i = 0; i <= triangleAmount;i++) 
	{ 
		glVertex2f(
			x + (radius * cos(i *  twicePi / triangleAmount)), 
			y + (radius * sin(i * twicePi / triangleAmount))
			);
	}
	glEnd();
}

void RenderUtility::DrawStrokeLines(std::vector<std::vector<CVSystem::MyLine>> uStrokeLines, std::vector<QRgb> uStrokeColors)
{
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	bool colorChange = false;	
	for(size_t a = 0; a < uStrokeLines.size(); a++)
	{
		std::vector<CVSystem::MyLine> segment = uStrokeLines[a];
		QColor col(uStrokeColors[a]);

		CVSystem::MyLine curLine;
		CVSystem::MyLine prevLine;
		CVSystem::MyLine nextLine;
		for(size_t step = 0; step < segment.size(); step++)
		{
			prevLine = curLine;
			curLine = CVSystem::MyLine(segment[step].XA, segment[step].YA, segment[step].XB, segment[step].YB);
			if(step < segment.size() - 1) nextLine = CVSystem::MyLine(segment[step + 1].XA, segment[step + 1].YA, segment[step + 1].XB, segment[step + 1].YB);
			else nextLine = CVSystem::MyLine(); 

			CVSystem::MyPoint pA;
			CVSystem::MyPoint pB;
			CVSystem::MyPoint pC;
			CVSystem::MyPoint pD;

			UtilityFunctions::GetSegmentPoints(curLine, prevLine, nextLine, 15, 15, &pA, &pB, &pC, &pD);

			glColor4f(col.red() / 255.0, col.green() / 255.0, col.blue() / 255.0, 1.0);

			// ADC
			glBegin(GL_POLYGON);
			glVertex2f(pA.x, pA.y); glVertex2f(pD.x, pD.y); glVertex2f(pC.x, pC.y);
			glEnd();

			// ABD
			glBegin(GL_POLYGON);
			glVertex2f(pA.x, pA.y); glVertex2f(pB.x, pB.y); glVertex2f(pD.x, pD.y);
			glEnd();
		}
	}

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

void RenderUtility::DrawWhiteRectangle(int width, int height)
{
	glDepthMask(GL_FALSE);	
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBegin(GL_QUADS);

	// text coords are flipped in y axis
	glTexCoord2d(0,1); glVertex3d(0,     0,      0);
	glTexCoord2d(1,1); glVertex3d(width, 0, 	 0);
	glTexCoord2d(1,0); glVertex3d(width, height, 0);
	glTexCoord2d(0,0); glVertex3d(0,	 height, 0);

	glEnd();
	glDepthMask(GL_TRUE);  
}

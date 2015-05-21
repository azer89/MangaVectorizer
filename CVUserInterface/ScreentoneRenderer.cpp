
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "ScreentoneRenderer.h"

ScreentoneRenderer::ScreentoneRenderer() : 
	_zoomFactor(1.0),
	_frameScale(3.0)
{	
}

ScreentoneRenderer::~ScreentoneRenderer()
{
}

void ScreentoneRenderer::SetTriangles(std::vector<CVSystem::MyPoint>			vertexList,
									   std::vector<CVSystem::MyIndexedTriangle> scTriangles, 
									   std::vector<CVSystem::MyIndexedTriangle> borderSCTriangles)
{
	this->_vertexList = vertexList;
	this->_triangles.insert( this->_triangles.end(), scTriangles.begin(), scTriangles.end() );
	this->_triangles.insert( this->_triangles.end(), borderSCTriangles.begin(), borderSCTriangles.end() );
}

void ScreentoneRenderer::SetParams(std::vector<double> params)
{
	if((int)params[0] == 0)
	{
		scShaderProgram1.addShaderFromSourceFile(QGLShader::Fragment, "Dots.fsh");
		scShaderProgram1.link();

		scShaderProgram2.addShaderFromSourceFile(QGLShader::Fragment, "DotsWithDensity.fsh");
		scShaderProgram2.link();
	}
	else if((int)params[0] == 1)
	{
		scShaderProgram1.addShaderFromSourceFile(QGLShader::Fragment, "Stripes.fsh");
		scShaderProgram1.link();

		scShaderProgram2.addShaderFromSourceFile(QGLShader::Fragment, "StripesWithDensity.fsh");
		scShaderProgram2.link();
	}
	else if((int)params[0] == 2)
	{
		scShaderProgram1.addShaderFromSourceFile(QGLShader::Fragment, "Grid.fsh");
		scShaderProgram1.link();

		scShaderProgram2.addShaderFromSourceFile(QGLShader::Fragment, "GridWithDensity.fsh");
		scShaderProgram2.link();
	}
	else
	{
		std::cout << "Not implemented!\n";
	}
	this->params = params;

	blurShaderProgram.addShaderFromSourceFile(QGLShader::Fragment, "GaussianBlur.fsh");
	blurShaderProgram.link();
}

void ScreentoneRenderer::DrawScreentone(double scaleFactor)
{
	float tOrientation = 0;

	// default value
	float tWidth = 4.0 * SystemParams::sr_tone_frequency;
	float tGap = 17.0 * SystemParams::sr_tone_frequency;

	if(params.size() != 0)
	{
		tOrientation = UtilityFunctions::DegreeToRadian(this->params[1]);
		tWidth       = this->params[2] * SystemParams::sr_tone_frequency;
		tGap         = this->params[3] * SystemParams::sr_tone_frequency;
	}

	if(SystemParams::sr_fixed)
	{
		tWidth /= _zoomFactor;
		tGap   /= _zoomFactor;
	}
	
	QGLShaderProgram* shaderProgram = &scShaderProgram1;

	shaderProgram->bind();

	int blackness = SystemParams::sr_blackness;
	shaderProgram->setUniformValue("whiteColor", QColor(Qt::white));
	shaderProgram->setUniformValue("blackColor", QColor(blackness, blackness, blackness, 255));

	shaderProgram->setUniformValue("cosO", (GLfloat)cos(tOrientation));
	shaderProgram->setUniformValue("sinO", (GLfloat)sin(tOrientation));
	shaderProgram->setUniformValue("width", (GLfloat)tWidth);
	shaderProgram->setUniformValue("gap", (GLfloat)tGap);

	shaderProgram->setUniformValue("imgWidth",  (GLfloat)_img_width);
	shaderProgram->setUniformValue("imgHeight", (GLfloat)_img_height);

	shaderProgram->setUniformValue("aaValue", (GLfloat)SystemParams::sr_aa_value);

	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	/*glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);*/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_TRIANGLES);

	for(size_t a = 0; a < _triangles.size(); a++)
	{
		glColor4f(1.0, 0.0, 0.0, 1.0);

		CVSystem::MyPoint oriP0 = _vertexList[_triangles[a].idx0];
		CVSystem::MyPoint oriP1 = _vertexList[_triangles[a].idx1];
		CVSystem::MyPoint oriP2 = _vertexList[_triangles[a].idx2];

		CVSystem::MyPoint p0 = oriP0 * scaleFactor;
		CVSystem::MyPoint p1 = oriP1 * scaleFactor;
		CVSystem::MyPoint p2 = oriP2 * scaleFactor;

		glTexCoord2d(oriP0.x / (double)_img_width, (_img_height - oriP0.y) / (double)_img_height); 
		glVertex3f(p0.x, p0.y, 0);	

		glTexCoord2d(oriP1.x / (double)_img_width, (_img_height - oriP1.y) / (double)_img_height); 
		glVertex3f(p1.x, p1.y, 0);  

		glTexCoord2d(oriP2.x / (double)_img_width, (_img_height - oriP2.y) / (double)_img_height); 
		glVertex3f(p2.x, p2.y, 0);
	}

	glEnd();

	glFlush();
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);   

	shaderProgram->release();
}


void ScreentoneRenderer::DrawScreentoneWithLighting(double scaleFactor)
{
	float tOrientation = 0;

	// default value
	float tWidth = 4.0 * SystemParams::sr_tone_frequency;
	float tGap = 17.0 * SystemParams::sr_tone_frequency;


	if(params.size() != 0)
	{
		tOrientation = UtilityFunctions::DegreeToRadian(this->params[1]);
		tWidth = this->params[2] * SystemParams::sr_tone_frequency;
		tGap = this->params[3] * SystemParams::sr_tone_frequency;
	}

	if(SystemParams::sr_fixed)
	{
		tWidth /= _zoomFactor;
		tGap   /= _zoomFactor;
	}

	//tWidth /= _frameScale;
	//tGap   /= _frameScale;

	QGLShaderProgram* shaderProgram = &scShaderProgram2;

	shaderProgram->bind();

	int blackness = SystemParams::sr_blackness;
	shaderProgram->setUniformValue("whiteColor", QColor(Qt::white));
	shaderProgram->setUniformValue("blackColor", QColor(blackness, blackness, blackness, 255));

	shaderProgram->setUniformValue("cosO", (GLfloat)cos(tOrientation));
	shaderProgram->setUniformValue("sinO", (GLfloat)sin(tOrientation));
	shaderProgram->setUniformValue("width", (GLfloat)tWidth);
	shaderProgram->setUniformValue("gap", (GLfloat)tGap);

	shaderProgram->setUniformValue("imgWidth",  (GLfloat)_img_width);
	shaderProgram->setUniformValue("imgHeight", (GLfloat)_img_height);

	shaderProgram->setUniformValue("aaValue", (GLfloat)SystemParams::sr_aa_value);

	shaderProgram->setUniformValue("thicknessFactor", (GLfloat)_thicknessFactor);

	shaderProgram->setUniformValue("tex", _gradationImgID);
	
	glEnable(GL_TEXTURE_2D);

	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	glBindTexture( GL_TEXTURE_2D, _gradationImgID );

	glBegin(GL_TRIANGLES);

	for(size_t a = 0; a < _triangles.size(); a++)
	{
		glColor4f(1.0, 0.0, 0.0, 1.0);

		CVSystem::MyPoint oriP0 = _vertexList[_triangles[a].idx0];
		CVSystem::MyPoint oriP1 = _vertexList[_triangles[a].idx1];
		CVSystem::MyPoint oriP2 = _vertexList[_triangles[a].idx2];

		CVSystem::MyPoint p0 = oriP0 * scaleFactor;
		CVSystem::MyPoint p1 = oriP1 * scaleFactor;
		CVSystem::MyPoint p2 = oriP2 * scaleFactor;

		glTexCoord2d(oriP0.x / (double)_img_width, (_img_height - oriP0.y) / (double)_img_height); 
		glVertex3f(p0.x, p0.y, 0);	

		glTexCoord2d(oriP1.x / (double)_img_width, (_img_height - oriP1.y) / (double)_img_height); 
		glVertex3f(p1.x, p1.y, 0);  

		glTexCoord2d(oriP2.x / (double)_img_width, (_img_height - oriP2.y) / (double)_img_height); 
		glVertex3f(p2.x, p2.y, 0);
	}

	glEnd();

	glFlush();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);  

	shaderProgram->release();
}

void ScreentoneRenderer::RenderFBOWithBlur(int x_offset, 
											int y_offset, 
											int frame_width, 
											int frame_height, 
											double zoom_factor, 
											QGLFramebufferObject* frameBuffer, 
											double blurFactor, 
											bool xAxis)
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
	blurShaderProgram.setUniformValue("radius", (GLfloat)(SystemParams::sr_blur) );

	if(xAxis) 
	{ 		
		blurShaderProgram.setUniformValue("dirx", (GLfloat)1.0); 
		blurShaderProgram.setUniformValue("diry", (GLfloat)0.0); 
	}
	else 
	{ 		
		blurShaderProgram.setUniformValue("dirx", (GLfloat)0.0); 
		blurShaderProgram.setUniformValue("diry", (GLfloat)1.0); 
	}

	blurShaderProgram.setUniformValue("resolutionx", (GLfloat)frame_width); 
	blurShaderProgram.setUniformValue("resolutiony", (GLfloat)frame_height);

	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

void ScreentoneRenderer::SetVariables(double img_width, double img_height)
{
	this->_img_width = img_width;
	this->_img_height = img_height;
}

QGLFramebufferObject* ScreentoneRenderer::RenderToFBO(int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor)
{
	this->_zoomFactor = zoom_factor;

	this->_ori_frame_width  = frame_width;
	this->_ori_frame_height = frame_height;
	this->_ori_x_offset		= x_offset;
	this->_ori_y_offset		= y_offset;

	
	QGLFramebufferObject* frameBufferC = new QGLFramebufferObject(frame_width, frame_height);

	frameBufferC->bind();
	DrawScreentone(1.0);
	frameBufferC->release();

	return frameBufferC;
}

QGLFramebufferObject* ScreentoneRenderer::RenderToFBOWithLighting(GLuint gradationImgID, double thicknessFactor, int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor)
{
	this->_gradationImgID = gradationImgID;
	this->_thicknessFactor = thicknessFactor;
	this->_zoomFactor = zoom_factor;

	this->_ori_frame_width  = frame_width;
	this->_ori_frame_height = frame_height;
	this->_ori_x_offset		= x_offset;
	this->_ori_y_offset		= y_offset;

	
	QGLFramebufferObject* frameBufferC = new QGLFramebufferObject(frame_width, frame_height);

	frameBufferC->bind();
	DrawScreentoneWithLighting(1.0);
	frameBufferC->release();

	return frameBufferC;
}

QGLFramebufferObject* ScreentoneRenderer::RenderToFBOAA(int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor)
{
	this->_zoomFactor = zoom_factor;

	this->_ori_frame_width  = frame_width;
	this->_ori_frame_height = frame_height;
	this->_ori_x_offset		= x_offset;
	this->_ori_y_offset		= y_offset;

	// Resize frame
	frame_width  *= _frameScale;
	frame_height *= _frameScale;
	x_offset     *= _frameScale;
	y_offset     *= _frameScale;	
	glViewport(0, 0, frame_width,  frame_height);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0 + x_offset, 
			   frame_width + x_offset,  
			   frame_height + y_offset,  
			   0 + y_offset);

	QGLFramebufferObject frameBufferA(frame_width, frame_height);
	QGLFramebufferObject frameBufferB(frame_width, frame_height);
	QGLFramebufferObject* frameBufferC = new QGLFramebufferObject(frame_width, frame_height);
	
	frameBufferA.bind();
	DrawScreentone(_frameScale);
	frameBufferA.release();	

	frameBufferB.bind();
	RenderFBOWithBlur(x_offset, y_offset, frame_width, frame_height, this->_zoomFactor , &frameBufferA, SystemParams::sr_blur, true);
	frameBufferB.release();
	frameBufferC->bind();
	RenderFBOWithBlur(x_offset, y_offset, frame_width, frame_height, this->_zoomFactor , &frameBufferB, SystemParams::sr_blur, false);
	frameBufferC->release();

	glPopMatrix();

	// Revert back
	frame_width  = _ori_frame_width;
	frame_height = _ori_frame_height;
	x_offset     = _ori_x_offset;
	y_offset     = _ori_y_offset;
	glViewport(0, 0, frame_width,  frame_height);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0 + x_offset, 
			   frame_width + x_offset,  
			   frame_height + y_offset,  
			   0 + y_offset);

	return frameBufferC;
}

QGLFramebufferObject* ScreentoneRenderer::RenderToFBOWithLightingAA(GLuint gradationImgID, double thicknessFactor, int x_offset, int y_offset, int frame_width, int frame_height, double zoom_factor)
{
	this->_gradationImgID = gradationImgID;
	this->_thicknessFactor = thicknessFactor;
	this->_zoomFactor = zoom_factor;

	this->_ori_frame_width  = frame_width;
	this->_ori_frame_height = frame_height;
	this->_ori_x_offset		= x_offset;
	this->_ori_y_offset		= y_offset;

	// Resize frame
	frame_width  *= _frameScale;
	frame_height *= _frameScale;
	x_offset     *= _frameScale;
	y_offset     *= _frameScale;	
	glViewport(0, 0, frame_width,  frame_height);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0 + x_offset, 
			   frame_width + x_offset,  
			   frame_height + y_offset,  
			   0 + y_offset);

	QGLFramebufferObject frameBufferA(frame_width, frame_height);
	QGLFramebufferObject frameBufferB(frame_width, frame_height);
	QGLFramebufferObject* frameBufferC = new QGLFramebufferObject(frame_width, frame_height);

	frameBufferA.bind();
	DrawScreentoneWithLighting(_frameScale);
	frameBufferA.release();

	frameBufferB.bind();
	RenderFBOWithBlur(x_offset, y_offset, frame_width, frame_height, zoom_factor, &frameBufferA, SystemParams::sr_blur, true);
	frameBufferB.release();
	frameBufferC->bind();
	RenderFBOWithBlur(x_offset, y_offset, frame_width, frame_height, zoom_factor, &frameBufferB, SystemParams::sr_blur, false);
	frameBufferC->release();

	// Revert back
	frame_width  = _ori_frame_width;
	frame_height = _ori_frame_height;
	x_offset     = _ori_x_offset;
	y_offset     = _ori_y_offset;
	glViewport(0, 0, frame_width,  frame_height);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0 + x_offset, 
			   frame_width + x_offset,  
			   frame_height + y_offset,  
			   0 + y_offset);

	return frameBufferC;
}


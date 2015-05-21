
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "LightingEffect.h"


LightingEffect::LightingEffect() : 
	_scaleFactor(10),
	_thicknessFactor(1.0),
	_mouseDown(false), 
	_iconPosition(CVSystem::MyPoint())
{
	Init();
}


LightingEffect::~LightingEffect()
{
}


void LightingEffect::paintGL()
{
	//DrawTexture(_imgID, _img_width, _img_height);
	if(!_iconPosition.Invalid())  { DrawLightIcon(_imgIconID, 64 / _zoom_factor, 64 / _zoom_factor, _iconPosition.x, _iconPosition.y); }
}


void LightingEffect::DrawLightIcon(GLuint texId, double width, double height, double xPos, double yPos)
{
	int x = xPos - (width / 2);
	int y = yPos - (height / 2);

	glEnable(GL_TEXTURE_2D);

	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	glBindTexture( GL_TEXTURE_2D, texId );

	glBegin(GL_QUADS);

	// text coords are flipped in y axis
	glTexCoord2d(0,1); glVertex3d(x,	     y,			   0);
	glTexCoord2d(1,1); glVertex3d(x + width, y,			   0);
	glTexCoord2d(1,0); glVertex3d(x + width, y + height,   0);
	glTexCoord2d(0,0); glVertex3d(x,	     y + height,   0);

	glEnd();

	glFlush();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);  
}


void LightingEffect::DrawTexture(GLuint texId, double width, double height)
{
	glEnable(GL_TEXTURE_2D);

	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	glBindTexture( GL_TEXTURE_2D, texId );

	glBegin(GL_QUADS);

	// text coords are flipped in y axis
	glTexCoord2d(0,1); glVertex3d(0,	 0,      0);
	glTexCoord2d(1,1); glVertex3d(width, 0,      0);
	glTexCoord2d(1,0); glVertex3d(width, height, 0);
	glTexCoord2d(0,0); glVertex3d(0,	 height, 0);

	glEnd();

	glFlush();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);  
}


void LightingEffect::mousePressEvent(int x, int y)
{
	_mouseDown = true;
}


void LightingEffect::mouseMoveEvent(int x, int y)
{
	if(!_mouseDown) {return;}
	_iconPosition.x = x;
	_iconPosition.y = y;

	if(!_iconPosition.Invalid())
	{
		QLinearGradient gradationColor( QPointF(x / _scaleFactor, y / _scaleFactor), QPoint( _img_width / 2, _img_height / 2 ) );
		gradationColor.setColorAt( 0, Qt::black );
		gradationColor.setColorAt( 1, Qt::white );

		QBrush brush(gradationColor);

		QPainter painter( &_imgOriginal );

		painter.setBrush( brush );
		painter.drawRect( _imgOriginal.rect() );

		_imgGL = _imgOriginal.scaled(_imgGL.width(), _imgGL.height(), Qt::IgnoreAspectRatio);
		_imgGL = QGLWidget::convertToGLFormat(_imgGL);

		glGenTextures(1, &_imgID);
		glBindTexture( GL_TEXTURE_2D, _imgID );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _imgGL.width(), _imgGL.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _imgGL.bits());
	}
}


void LightingEffect::mouseReleaseEvent(int x, int y)
{
	_mouseDown = false;
}


void LightingEffect::Init()
{
	QString filename = "Image\\light.png";

	_imgIconOriginal.load(filename);

	int w = _imgIconOriginal.width();
	int h = _imgIconOriginal.height();

	_imgIconGL = QGLWidget::convertToGLFormat(_imgIconOriginal);

	glGenTextures(1, &_imgIconID);
	glBindTexture( GL_TEXTURE_2D, _imgIconID );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _imgIconGL.width(), _imgIconGL.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _imgIconGL.bits());
	
}


void LightingEffect::SetUISystem(int img_width, int img_height)
{
	this->_img_width = img_width / _scaleFactor;
	this->_img_height = img_height / _scaleFactor;

	// calculating power-of-two (pow) size
	int xpow = (int) std::pow(2.0, std::ceil( std::log10((double)_img_width )/std::log10(2.0) ) );
	int ypow = (int) std::pow(2.0, std::ceil( std::log10((double)_img_height )/std::log10(2.0) ) );

	xpow = my_max(xpow, ypow);	// the texture should be square too
	xpow = my_min(xpow, 1024);	// shrink if the size is too big
	ypow = xpow;

	_imgOriginal = QImage(_img_width, _img_height, QImage::Format_ARGB32);
	for(int a = 0; a < _img_width; a++)
	{
		for(int b = 0; b < _img_height; b++)
		{ _imgOriginal.setPixel(a, b, qRgba(255, 255, 255, 225)); }
	}

	// transform the image to square pow size
	_imgGL = _imgOriginal.scaled(xpow, ypow, Qt::IgnoreAspectRatio);
	_imgGL = QGLWidget::convertToGLFormat(_imgGL);

	glGenTextures(1, &_imgID);
	glBindTexture( GL_TEXTURE_2D, _imgID );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _imgGL.width(), _imgGL.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _imgGL.bits());
	
	/*
	_imgTOriginal = QImage(_img_width, _img_height, QImage::Format_ARGB32);
	_imgTOriginal.fill(Qt::black);
	for(size_t a = 0; a < _scTriangles.size(); a++)
	{
		QPolygonF polygon;
		polygon << QPointF(_vertexList[_scTriangles[a].idx0].x, _vertexList[_scTriangles[a].idx0].y);
		polygon << QPointF(_vertexList[_scTriangles[a].idx1].x, _vertexList[_scTriangles[a].idx1].y);
		polygon << QPointF(_vertexList[_scTriangles[a].idx2].x, _vertexList[_scTriangles[a].idx2].y);

		QPainter painter(&_imgTOriginal);
		painter.setPen(Qt::NoPen);
		painter.setBrush(Qt::white);
		painter.drawPolygon(polygon);
	}
	for(size_t a = 0; a < _borderSCTriangles.size(); a++)
	{
		QPolygonF polygon;
		polygon << QPointF(_vertexList[_borderSCTriangles[a].idx0].x, _vertexList[_borderSCTriangles[a].idx0].y);
		polygon << QPointF(_vertexList[_borderSCTriangles[a].idx1].x, _vertexList[_borderSCTriangles[a].idx1].y);
		polygon << QPointF(_vertexList[_borderSCTriangles[a].idx2].x, _vertexList[_borderSCTriangles[a].idx2].y);

		QPainter painter(&_imgTOriginal);
		painter.setPen(Qt::NoPen);
		painter.setBrush(Qt::white);
		painter.drawPolygon(polygon);
	}

	_imgTGL = _imgTOriginal.scaled(xpow, ypow, Qt::IgnoreAspectRatio);
	_imgTGL = QGLWidget::convertToGLFormat(_imgTGL);

	glGenTextures(1, &_imgTID);
	glBindTexture( GL_TEXTURE_2D, _imgTID );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _imgTGL.width(), _imgTGL.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _imgTGL.bits());
	*/
}


void LightingEffect::SetVariables(int _x_offset, int _y_offset, int _frame_width, int _frame_height, /*int _img_width, int _img_height,*/ double _zoom_factor)
{
	this->_x_offset     = _x_offset;
	this->_y_offset     = _y_offset;

	this->_frame_width  = _frame_width;
	this->_frame_height = _frame_height;
	this->_zoom_factor  = _zoom_factor;
}

/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "GLWidget.h"

#include "MeshDeformer.h"
#include "RenderUtility.h"
#include "UtilityFunctions.h"
#include "LightingEffect.h"
#include "CurveRenderer1.h"
#include "CurveRenderer2.h"
#include "ScreentoneRenderer.h"
#include "PartExtractor.h"

#include "DeformationManager.h"


GLWidget::GLWidget(QGLFormat format, QWidget *parent) :
	QGLWidget(format, parent),

	// local variables
	_drawMode(DRAW_RASTER), _isMouseDown(false), _zoomFactor(1.0),
	
	// curve and screentone renderer
	_lsCRenderer(0), _scRenderer(0), _cRendererV2(0),
	
	// For initialization please check initializeGL() function
	// mesh deform fix
	//_meshDeformer(0), 
	_deformManager(0),
	
	_renderUtility(0), _partExtractor(0), _lightingEffect(0),

	_img_width(0), _img_height(0)
{
}

GLWidget::~GLWidget()
{	
	if(_lsCRenderer) { delete _lsCRenderer; }
	if(_cRendererV2) { delete _cRendererV2; }

	if(_scRenderer) { delete _scRenderer; }
	
	// mesh deform fix
	if(_deformManager) { delete _deformManager; }
	//if(_meshDeformer) delete _meshDeformer;
	
	if(_renderUtility) { delete _renderUtility; }
	//if(_curveCompletionUI) delete _curveCompletionUI;
	if(_lightingEffect) { delete _lightingEffect; }
}



void GLWidget::initializeGL() 
{	
}

/**
 * SET
 */
void GLWidget::SetWidthHeight(int img_width, int img_height)
{
	this->_img_width = img_width;
	this->_img_height = img_height;

	_lightingEffect->SetUISystem(this->_img_width, this->_img_height);
}

/**
 * SET
 */
void GLWidget::SetLSQuads(std::vector<CVSystem::MyQuad> lsQuads) 
{ 
	if(_lsCRenderer) { delete _lsCRenderer; }
	_lsCRenderer = new CurveRenderer1();
	_lsCRenderer->Init();

	this->_lsCRenderer->SetQuads(lsQuads); 
	this->lsQuads = lsQuads;
}	

/**
 * SET
 */
void GLWidget::SetScreentoneParams(std::vector<double> params)
{
	// screentone renderer	
	if(_scRenderer) {delete _scRenderer;}
	_scRenderer = new ScreentoneRenderer();
	_scRenderer->SetTriangles(_vertexList, _scTriangles, _borderSCTriangles);
	_scRenderer->SetParams(params);
	_scRenderer->SetVariables(_img_width, _img_height);

	// mesh deform fix
	_deformManager->SetScreentoneSystem(params, _img_width, _img_height);
	//_meshDeformer->SetScreentoneSystem(params, _img_width, _img_height);
}

/**
 * SET
 */
void GLWidget::SetScreentoneOrientation(double degAngle)
{
	if(_scRenderer)
	{
		_scRenderer->SetOrientation(degAngle);
		// mesh deform fix
		_deformManager->SetScreentoneOrientation(degAngle);
		//_meshDeformer->GetScreentoneRenderer()->SetOrientation(degAngle);
	}
}

/**
 * SET
 */
void GLWidget::SetTriangles(std::vector<int> offsets,
							std::vector<CVSystem::MyPoint> vertexList,
							std::vector<CVSystem::MyIndexedTriangle> indexedTriangles,
							std::vector<CVSystem::MyIndexedTriangle> borderSCTriangles,
							std::vector<CVSystem::MyIndexedTriangle> borderWTriangles,
							std::vector<CVSystem::MyIndexedTriangle> scTriangles,
							std::vector<CVSystem::MyIndexedTriangle> wTriangles,
							std::vector<CVSystem::MyIndexedTriangle> bTriangles)
{
	this->_offsets			 = offsets;

	this->_vertexList		 = vertexList;
	this->_indexedTriangles  = indexedTriangles;

	this->_borderSCTriangles = borderSCTriangles;
	this->_borderWTriangles  = borderWTriangles;

	this->_scTriangles = scTriangles;
	this->_wTriangles  = wTriangles;
	this->_bTriangles  = bTriangles;

	
	std::cout << "the number of entire vertices: " << _vertexList.size() << "\n";
	std::cout << "the number of entire triangles: " << _indexedTriangles.size() << "\n";
	std::cout << "the number of border triangles: " << _borderSCTriangles.size() + _borderWTriangles.size() << "\n";

	// mesh deform fix
	if(_deformManager) {delete _deformManager;}
	_deformManager = new DeformationManager();
	_deformManager->SetTriangleData(offsets, 
								vertexList, 
								indexedTriangles, 
								borderSCTriangles, 
								borderWTriangles, 
								scTriangles, 
								wTriangles, 
								bTriangles);

	if(_renderUtility) {delete _renderUtility;}
	_renderUtility  = new RenderUtility();

	if(_lightingEffect) {delete _lightingEffect;}
	_lightingEffect = new LightingEffect();
}

/*
 * SET
 * This function should be called after SetTriangles()
 */
void GLWidget::SetIndexedBezierCurves(std::vector<CVSystem::MyIndexedBezierCurves> indexedBezierCurves)
{
	this->_indexedBezierCurves = indexedBezierCurves;

	if(_cRendererV2) { delete _cRendererV2; }
	_cRendererV2 = new CurveRenderer2();
	_cRendererV2->Init();
	_cRendererV2->SetBezierCurves(_indexedBezierCurves, _vertexList);
	
	// mesh deform fix
	_deformManager->SetIndexedBezierCurves(_indexedBezierCurves);
	//_meshDeformer->SetIndexedBezierCurves(_indexedBezierCurves);

	if(_partExtractor) {delete _partExtractor; }
	_partExtractor  = new PartExtractor();
	_partExtractor->SetData(_vertexList, _indexedBezierCurves, _bTriangles);

	int numBz = 0;
	for(size_t a = 0; a < _indexedBezierCurves.size(); a++)
	{
		numBz += (_indexedBezierCurves[a].indices.size() - 1);
	}

	std::cout << "Number of bezier curves: " << numBz << "\n";
}

bool GLWidget::event( QEvent * event ) { return QGLWidget::event(event); }

// This is an override function from Qt but I can't find its purpose
/* void GLWidget::resizeGL(int width, int height)  { QGLWidget::resizeGL(width, height); }*/

void GLWidget::paintGL() 
{	
	//glClearColor(0.5, 0.5, 0.5, 1.0);
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glViewport(0, 0, this->width(),  this->height());	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// scrollOffset has the coordinates of horizontal and vertical scrollbars	
	gluOrtho2D(0 + _scrollOffset.x(), 
			   this->width() + _scrollOffset.x(),  
			   this->height() + _scrollOffset.y(),  
			   0 + _scrollOffset.y());	// flip the y axis
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
	glScalef(_zoomFactor, _zoomFactor, _zoomFactor);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if(_drawMode == DRAW_RASTER)
	{
		_renderUtility->DrawTexture(_imgID, _img_width, _img_height);
		if(SystemParams::gr_user_scribbles) { _renderUtility->DrawStrokeLines(_uStrokeLines, _uStrokeColors); }
		if(SystemParams::gr_sc_regions){_renderUtility->DrawTexture(_segmentID, _segmentOriginal.width(), _segmentOriginal.height());}
	}
	else if(_drawMode == DRAW_VECTOR)
	{
		if(SystemParams::gr_gpu) { DrawMeshWithGPU(); }
		else { DrawMeshWithoutGPU(); }
	}
	else if(_drawMode == DRAW_DEFORMATION)
	{
		if(_deformManager) 
		{
			_deformManager->SetVariables(_scrollOffset.x(), _scrollOffset.y(), this->width(), this->height(), _zoomFactor);
			_deformManager->paintGL();
		}
	}
	else if(_drawMode == DRAW_PART_EXTRACTION)
	{
		if(SystemParams::gr_gpu) { DrawMeshWithGPU(); }
		else { DrawMeshWithoutGPU(); }

		if(SystemParams::gr_show_wireframe) { _renderUtility->DrawWireframes(_vertexList, _indexedTriangles, 0, 174.0 / 255.0, 239.0 / 255.0); }

		_partExtractor->paintGL();
	}
	else if(_drawMode == DRAW_LIGHTING_EFFECT)
	{
		
		if(SystemParams::gr_gpu) { DrawLightingEffect(); }
		else { DrawMeshWithoutGPU(); }
		if(SystemParams::gr_show_wireframe) { _renderUtility->DrawWireframes(_vertexList, _indexedTriangles, 0, 0, 1); }

		_lightingEffect->SetVariables(_scrollOffset.x(), _scrollOffset.y(), this->width(), this->height(), /*this->_img_width, this->_img_height,*/ _zoomFactor);
		_lightingEffect->paintGL();
	}
}

void GLWidget::DrawMeshWithoutGPU()
{
	//_renderUtility->DrawTriangles(_vertexList, _wTriangles, 1.0, 1.0, 1.0);
	//_renderUtility->DrawTriangles(_vertexList, _borderWTriangles, 1.0, 1.0, 1.0);
	//_renderUtility->DrawTriangles(_vertexList, _scTriangles, 0.75, 0.75, 0.75);
	//double blackness = SystemParams::cr_blackness;
	//_renderUtility->DrawTriangles(_vertexList, _bTriangles, blackness, blackness, blackness);

	_renderUtility->DrawWireframes(_vertexList, _indexedTriangles, 0, 84.0 / 255.0, 166.0 / 255.0);
	DrawDebug();
}

void GLWidget::DrawMeshWithGPU()
{	
	//_cRendererV2->paintGL();

	//if(!_scRenderer || !_cRendererV2) {return;}
	
	QGLFramebufferObject* scFBO = 0;
	QGLFramebufferObject* crFBO = 0;

	if(_scRenderer)
	{
		scFBO = _scRenderer->RenderToFBOAA(_scrollOffset.x(), _scrollOffset.y(), this->width(), this->height(), this->_zoomFactor);
	}

	if(_cRendererV2)
	{
		crFBO = _cRendererV2->RenderCurvesToFBO(_scrollOffset.x(), _scrollOffset.y(), this->width(), this->height(), this->_zoomFactor);
	}

	_renderUtility->DrawFullscreen(crFBO, 
								   scFBO,
								   _vertexList,
								   _borderWTriangles,
								   _wTriangles,
								   _bTriangles,
								   _scrollOffset.x(), 
								   _scrollOffset.y(), 
								   this->width(), 
								   this->height(), 
								   this->_zoomFactor);

	

	if (scFBO) { delete scFBO; }
	if (crFBO) { delete crFBO; }
}

void GLWidget::DrawLightingEffect()
{
	//if(!_scRenderer || !_cRendererV2) {return;}

	QGLFramebufferObject* scFBO = 0;
	QGLFramebufferObject* crFBO = 0;

	if(_scRenderer)
	{
		scFBO = _scRenderer->RenderToFBOWithLightingAA(_lightingEffect->_imgID, _lightingEffect->GetThickness(), _scrollOffset.x(), _scrollOffset.y(), this->width(), this->height(), this->_zoomFactor);
	}

	if(_cRendererV2)
	{
		crFBO = _cRendererV2->RenderCurvesToFBO(_scrollOffset.x(), _scrollOffset.y(), this->width(), this->height(), this->_zoomFactor);
	}
	_renderUtility->DrawFullscreen(crFBO, 
								   scFBO,
								   _vertexList,
								   _borderWTriangles,
								   _wTriangles,
								   _bTriangles,
								   _scrollOffset.x(), 
								   _scrollOffset.y(), 
								   this->width(), 
								   this->height(), 
								   this->_zoomFactor);

	if (scFBO) { delete scFBO; }
	if (crFBO) { delete crFBO; }
}


void GLWidget::CreateNewStrokeSegment()
{
	int idx = _uStrokeLines.size() - 1;
	if(idx >= 0 && _uStrokeLines[idx].size() > 0) 
	{
		std::vector<CVSystem::MyLine> newSegment;
		_uStrokeLines.push_back(newSegment);		
		_uStrokeColors.push_back(qRgb(rand() % 255, rand() % 255, rand() % 255));
	}
}

// Mouse is pressed
void GLWidget::mousePressEvent(int x, int y)
{
	_isMouseDown = true;	

	x += _scrollOffset.x();	y += _scrollOffset.y();
	x /= _zoomFactor;		y /= _zoomFactor;

	// mesh deform fix
	//if(this->_drawMode == DRAW_DEFORMATION && _meshDeformer)  { _meshDeformer->mousePressEvent(x, y); }
	if(this->_drawMode == DRAW_DEFORMATION && _deformManager)  { _deformManager->mousePressEvent(x, y); }
	else if(this->_drawMode == DRAW_PART_EXTRACTION && _partExtractor)  { _partExtractor->mousePressEvent(x, y); }
	else if(this->_drawMode == DRAW_LIGHTING_EFFECT && _lightingEffect) {_lightingEffect->mousePressEvent(x, y);}

	if(x < 0 || y < 0 || x >= _img_width || y >= _img_height) { return; }

	if(_uStrokeLines.size() == 0)
	{
		std::vector<CVSystem::MyLine> newSegment;
		_uStrokeLines.push_back(newSegment);
		_uStrokeColors.push_back(qRgb(rand() % 255, rand() % 255, rand() % 255));
	}
	_uPrevPoint =  CVSystem::MyPoint(x, y);
}

// Mouse is moved
void GLWidget::mouseMoveEvent(int x, int y)
{	
	x += _scrollOffset.x();	y += _scrollOffset.y();
	x /= _zoomFactor;		y /= _zoomFactor;

	// mesh deform fix
	//if(this->_drawMode == DRAW_DEFORMATION && _meshDeformer) { _meshDeformer->mouseMoveEvent(x, y); }
	if(this->_drawMode == DRAW_DEFORMATION && _deformManager) { _deformManager->mouseMoveEvent(x, y); }
	if(this->_drawMode == DRAW_PART_EXTRACTION && _partExtractor)  { _partExtractor->mouseMoveEvent(x, y); }
	else if(this->_drawMode == DRAW_LIGHTING_EFFECT && _lightingEffect) {_lightingEffect->mouseMoveEvent(x, y);}

	if(x < 0 || y < 0 || x >= _img_width|| y >= _img_height|| !_isMouseDown) { return; }	

	if(_drawMode != DRAW_RASTER) { return; }

	int idx = _uStrokeLines.size() - 1;
	if(idx >= 0) 
	{
		_uStrokeLines[idx].push_back(CVSystem::MyLine(_uPrevPoint.x, _uPrevPoint.y, x, y));
		_uPrevPoint = CVSystem::MyPoint(x,y);
	}
}

// Mouse is released
void GLWidget::mouseReleaseEvent(int x, int y) 
{ 
	_isMouseDown = false;
	x += _scrollOffset.x();	y += _scrollOffset.y();
	x /= _zoomFactor;		y /= _zoomFactor;

	// mesh deform fix
	//if(this->_drawMode == DRAW_DEFORMATION && _meshDeformer)  { _meshDeformer->mouseReleaseEvent(x, y); }
	if(this->_drawMode == DRAW_DEFORMATION && _deformManager)  { _deformManager->mouseReleaseEvent(x, y); }
	else if(this->_drawMode == DRAW_PART_EXTRACTION && _partExtractor) { _partExtractor->mouseReleaseEvent(x, y); }
	else if(this->_drawMode == DRAW_LIGHTING_EFFECT && _lightingEffect) {_lightingEffect->mouseReleaseEvent(x, y);}

	//if(x < 0 || y < 0 || x >= imgOriginal.width()|| y >= imgOriginal.height()|| !isMouseDown) 
	//{ return; }
}

void GLWidget::mouseDoubleClick(int x, int y)
{
	x += _scrollOffset.x();	y += _scrollOffset.y();
	x /= _zoomFactor;		y /= _zoomFactor;

	if(this->_drawMode == DRAW_PART_EXTRACTION && _partExtractor)
		{ _partExtractor->DoPointLocationTest(x, y); }
}

// Set function
void GLWidget::SetSegment(int* labelMap)
{
	//this->drawMode = RASTER_WITH_SEGMENT;
	_segmentOriginal = QImage(_img_width, _img_height, QImage::Format_ARGB32);
	for(int a = 0; a < _segmentOriginal.width(); a++)
	{
		for(int b = 0; b < _segmentOriginal.height(); b++)
		{ _segmentOriginal.setPixel(a, b, qRgba(0, 0, 0, 0)); }
	}
	if(_uStrokeColors.size() == 0)
	{
		int numColor = UtilityFunctions::GetMaxValue(labelMap, _img_width * _img_height) + 1;
		for(int a = 0; a < numColor; a++)
		{ _uStrokeColors.push_back(qRgb(rand() % 255, rand() % 255, rand() % 255)); } 
	}
	for(int a = 0; a < _segmentOriginal.width(); a++)
	{
		for(int b = 0; b < _segmentOriginal.height(); b++)
		{			
			int idx = labelMap[a + b * _segmentOriginal.width()];
			if(idx != -1)
			{
				QColor col = _uStrokeColors[idx];
				_segmentOriginal.setPixel(a, b, qRgba(col.red(), col.green(), col.blue(), 225));
			}
		}
	}
	_segmentGL = _segmentOriginal.scaled(_imgGL.width(), _imgGL.height(), Qt::IgnoreAspectRatio);
	_segmentGL = QGLWidget::convertToGLFormat(_segmentGL);

	glGenTextures(1, &_segmentID);
	glBindTexture( GL_TEXTURE_2D, _segmentID );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _segmentGL.width(), _segmentGL.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _segmentGL.bits());

	this->updateGL();	// Update !
}

// Set function
void GLWidget::SetImage(QString img)
{
	this->Reset();
	_imgOriginal.load(img);
	
	// size
	this->_img_width = _imgOriginal.width();
	this->_img_height = _imgOriginal.height();

	// calculating power-of-two (pow) size
	int xpow = (int) std::pow(2.0, std::ceil( std::log10((double)_img_width )/std::log10(2.0) ) );
	int ypow = (int) std::pow(2.0, std::ceil( std::log10((double)_img_height )/std::log10(2.0) ) );

	xpow = my_max(xpow, ypow);	// the texture should be square too
	xpow = my_min(xpow, 1024);	// shrink if the size is too big
	ypow = xpow;

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
	
	this->updateGL(); // Update !
}

// Save image to file
void GLWidget::SaveImage(QString filename)
{	
	// Save onscreen
	paintGL();
	glFlush();	
	QImage image = this->grabFrameBuffer();
	if( !image.save( filename) ) std::cout << "Error saving image\n";

	// Save triangle only (OpenCV)	
	/*
	double maxZoom = 1.0;
	int w = img_width * maxZoom;
	int h = img_height * maxZoom;
	cv::Mat imgMat(h, w, CV_8UC1);
	imgMat = 255;

	for(size_t a = 0; a < lsTriangles.size(); a++)
	{
		CVSystem::MyTriangle t = lsTriangles[a];
		std::vector<cv::Point> tri(3);
		tri[0] = cv::Point(t.p1.x * maxZoom, t.p1.y * maxZoom);
		tri[1] = cv::Point(t.p2.x * maxZoom, t.p2.y * maxZoom);
		tri[2] = cv::Point(t.p3.x * maxZoom, t.p3.y * maxZoom);
		
		const cv::Point* elementPoints[1] = { &tri[0] };
		int numberOfPoints = (int)tri.size();

		cv::fillPoly (imgMat, elementPoints, &numberOfPoints, 1, cv::Scalar(0), 8);
	}
	cv::imwrite(filename.toStdString(), imgMat);
	*/	
}

void GLWidget::SetDrawMode(DrawMode mode) { _drawMode = mode; }
void GLWidget::HorizontalScroll(int val) { _scrollOffset.setX(val); }
void GLWidget::VerticalScroll(int val) { _scrollOffset.setY(val); }
void GLWidget::ZoomIn() { this->_zoomFactor += 0.05f; }
void GLWidget::ZoomOut() { this->_zoomFactor -= 0.05f; if(this->_zoomFactor < 0.1f) _zoomFactor = 0.1f; }
void GLWidget::UndoStroke() { if(_uStrokeLines.size() > 0) _uStrokeLines.pop_back(); }
void GLWidget::ClearStroke() { _uStrokeLines.clear(); }
void GLWidget::DeleteSegment() { if(_uStrokeLines.size() > 0) _uStrokeLines.pop_back(); }


void GLWidget::DrawDebug()
{	
	using namespace CVSystem;

	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(2.0);
	glBegin(GL_LINES);	

	for(size_t a = 0; a < _indexedBezierCurves.size(); a++)
	{
		MyIndexedBezierCurves bCurves = _indexedBezierCurves[a];

		for(size_t b = 0; b < bCurves.indices.size() - 1; b++)
		{
			/*
			MyPoint pA = _vertexList[bCurves.indices[b]]; 
			MyPoint pB = _vertexList[bCurves.indices[b+1]];
			MyLine curLine(pA, pB);

			double mag = curLine.Magnitude();
			int N = mag / 5.0;

			std::vector<MyPoint> sampePts;
			UtilityFunctions::UniformResample(curLine, sampePts, N);
			for(size_t c = 1; c < sampePts.size(); c++)
			{
				double rr = rand() % 255;
				double gg = rand() % 255;
				double bb = rand() % 255;
				glColor4f(rr / 255.0, gg / 255.0, bb / 255.0, 1.0);

				CVSystem::MyPoint pA = sampePts[c-1]; 
				CVSystem::MyPoint pB = sampePts[c];
				glVertex3f(pA.x, pA.y, 0); 
				glVertex3f(pB.x, pB.y, 0);
			}*/
			
			
			/*if(b % 2 == 0)
			{
				double rr = rand() % 255;
				double gg = rand() % 255;
				double bb = rand() % 255;
				glColor4f(rr / 255.0, gg / 255.0, bb / 255.0, 1.0);
			}*/

			glColor4f(1, 0, 0, 1.0);
			CVSystem::MyPoint pA = _vertexList[bCurves.indices[b]]; 
			CVSystem::MyPoint pB = _vertexList[bCurves.indices[b+1]];
			glVertex3f(pA.x, pA.y, 0); glVertex3f(pB.x, pB.y, 0);
			
		}
	}
	glEnd();

	glFlush();
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);  
}

void GLWidget::Reset()
{
	if(_img_width != 0 && _img_height != 0) { glDeleteTextures( 1, &_imgID ); }

	this->_uStrokeColors.clear();		
	this->_uStrokeLines.clear();	

	this->_oTriangles.clear();	// should be deleted
	this->_lsTriangles.clear();	// should be deleted

	if(_lsCRenderer) _lsCRenderer->Reset();
}



/*
void GLWidget::DrawTexture(GLuint texId, double width, double height)
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
	glTexCoord2d(0,1); glVertex3d(0,	 0,          0);
	glTexCoord2d(1,1); glVertex3d(width, 0,          0);
	glTexCoord2d(1,0); glVertex3d(width, height, 0);
	glTexCoord2d(0,0); glVertex3d(0,	 height, 0);

	glEnd();

	glFlush();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);  
}*/

/*
// Set function
void GLWidget::SetDilatedSegment(int* labelMap)
{
	//this->drawMode = RASTER_WITH_SEGMENT;
	_segmentDilatedOriginal = QImage(_img_width, _img_height, QImage::Format_ARGB32);
	for(int a = 0; a < _segmentDilatedOriginal.width(); a++)
	{ for(int b = 0; b < _segmentDilatedOriginal.height(); b++)
	{ _segmentDilatedOriginal.setPixel(a, b, qRgba(0, 0, 0, 0)); } }

	if(_uStrokeColors.size() == 0)
	{
		int numColor = UtilityFunctions::GetMaxValue(labelMap, _img_width * _img_height) + 1;
		for(int a = 0; a < numColor; a++)
		{ _uStrokeColors.push_back(qRgb(rand() % 255, rand() % 255, rand() % 255)); }
	}

	for(int a = 0; a < _segmentDilatedOriginal.width(); a++)
	{
		for(int b = 0; b < _segmentDilatedOriginal.height(); b++)
		{			
			int idx = labelMap[a + b * _segmentDilatedOriginal.width()];
			if(idx != -1)
			{
				QColor col = _uStrokeColors[idx];
				_segmentDilatedOriginal.setPixel(a, b, qRgba(col.red(), col.green(), col.blue(), 225));
			}
		}
	}

	_segmentDilatedGL = _segmentDilatedOriginal.scaled(_imgGL.width(), _imgGL.height(), Qt::IgnoreAspectRatio);
	_segmentDilatedGL = QGLWidget::convertToGLFormat(_segmentDilatedGL);

	glGenTextures(1, &_segmentDilatedID);
	glBindTexture( GL_TEXTURE_2D, _segmentDilatedID );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _segmentDilatedGL.width(), _segmentDilatedGL.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _segmentDilatedGL.bits());

	// Update !
	this->updateGL();
}*/

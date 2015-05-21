
/**
 * OpenGL rendering for Qt
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __GL_Widget__
#define __GL_Widget__

#include <vector>

#include "MyPoint.h"
#include "MyIndexedTriangle.h"
#include "MyIndexedBezierCurves.h"
#include "MyLine.h"
#include "MyTriangle.h"
#include "MyPolygon.h"
#include "DrawMode.h"

// forward declaration
class DeformationManager;
class RenderUtility;
class LightingEffect;
class CurveRenderer1;
class CurveRenderer2;
class PartExtractor;
class ScreentoneRenderer;

class GLWidget : public QGLWidget
{
	Q_OBJECT

private:


	DeformationManager*	_deformManager;
	RenderUtility*		_renderUtility;
	LightingEffect*		_lightingEffect;
	CurveRenderer1*		_lsCRenderer;	// non indexed
	CurveRenderer2*		_cRendererV2;	// indexed	
	PartExtractor*		_partExtractor;
	ScreentoneRenderer* _scRenderer;

	// image size (do not depend on QImage)
	int _img_width;
	int _img_height;

	// image
	QImage _imgOriginal;
	QImage _imgGL;
	GLuint _imgID;

	// Segment
	QImage _segmentOriginal;
	QImage _segmentGL;
	GLuint _segmentID;

	bool		_isMouseDown;
	float		_zoomFactor;
	DrawMode	_drawMode;
	QPoint		_scrollOffset;

	std::vector<QRgb>						   _uStrokeColors;
	CVSystem::MyPoint						   _uPrevPoint;
	std::vector<std::vector<CVSystem::MyLine>> _uStrokeLines;

	std::vector<CVSystem::MyTriangle> _oTriangles;
	std::vector<CVSystem::MyTriangle> _lsTriangles;

	std::vector<int>						  _offsets;
	std::vector<CVSystem::MyPoint>			  _vertexList;
	std::vector<CVSystem::MyIndexedTriangle>  _indexedTriangles;
	std::vector<CVSystem::MyIndexedTriangle>  _borderSCTriangles;
	std::vector<CVSystem::MyIndexedTriangle>  _borderWTriangles;
	std::vector<CVSystem::MyIndexedTriangle>  _scTriangles;
	std::vector<CVSystem::MyIndexedTriangle>  _wTriangles;
	std::vector<CVSystem::MyIndexedTriangle>  _bTriangles;

	std::vector<CVSystem::MyIndexedBezierCurves> _indexedBezierCurves;

public:

	// GET functions
	std::vector<CVSystem::MyPoint>				 GetVertexList()		  { return _vertexList;        };
	std::vector<CVSystem::MyIndexedTriangle>	 GetIndexedTriangles()    { return _indexedTriangles;  };
	std::vector<CVSystem::MyIndexedTriangle>	 GetBorderSCtriangles()   { return _borderSCTriangles; };
	std::vector<CVSystem::MyIndexedTriangle>	 GetBorderWtriangles()    { return _borderWTriangles;  };
	std::vector<CVSystem::MyIndexedTriangle>	 GetSCtriangles()		  { return _scTriangles;       };
	std::vector<CVSystem::MyIndexedTriangle>	 GetWtriangles()		  { return _wTriangles;        };
	std::vector<CVSystem::MyIndexedTriangle>	 GetBTriangles()		  { return _bTriangles;        };
	std::vector<CVSystem::MyIndexedBezierCurves> GetIndexedBezierCurves() { return _indexedBezierCurves; }

	// constructor
	GLWidget( QGLFormat format, QWidget *parent = 0);
	// destructor
	~GLWidget();
	
	// set bitmap image
	void SetImage(QString img);	
	// set screentone segments
	void SetSegment(int* labelMap);
	// set screentone properties (for rendering)
	void SetScreentoneParams(std::vector<double> params);
	// set screentone orientation (for rendering)
	void SetScreentoneOrientation(double degAngle);

	// set the current draw mode
	void SetDrawMode(DrawMode mode);
	// get the current draw mode
	DrawMode GetDrawMode() { return _drawMode; }

	// save current buffer to image
	void SaveImage(QString filename);
	//  obtain image size
	QSize GetImageSize() { return /*imgOriginal.size();*/ QSize(_img_width, _img_height); }
			
	// zoom in handle
	void ZoomIn();
	// zoom out handle
	void ZoomOut();	
	// set zoom value
	void SetZoom(int val){this->_zoomFactor = val;}
	// get zoom value
	float GetZoomFactor() { return this->_zoomFactor; }
		
	// set horizontal scroll position
	void HorizontalScroll(int val);
	// set vertical scroll position
	void VerticalScroll(int val);
	// get scroll position (horizontal and vertical)
	QPoint GetScrollOffset() {return this->_scrollOffset;}

	// mesh deformation
	DeformationManager* GetDeformManager() { return this->_deformManager; }
	// get context completion instance
	PartExtractor*  GetPartExtractor()  { return this->_partExtractor; }
	// get lighting effect instance
	LightingEffect* GetLightingEffect() { return this->_lightingEffect; };

	// user stroke (for level set, not used anymore)
	void UndoStroke();
	// user stroke (for level set, not used anymore)
	void ClearStroke();
	// user stroke (for level set, not used anymore)
	void CreateNewStrokeSegment();
	// user stroke (for level set, not used anymore)
	void DeleteSegment();
	// user stroke (for level set, not used anymore)
	std::vector<std::vector<CVSystem::MyLine>> GetStrokes() { return _uStrokeLines;}

	// mouse press
	void mousePressEvent(int x, int y);
	// mouse move
	void mouseMoveEvent(int x, int y);
	// mouse release
	void mouseReleaseEvent(int x, int y);
	// mouse double click
	void mouseDoubleClick(int x, int y);

	// reset everything
	void Reset();

	// set image width and height
	void SetWidthHeight(int img_width, int img_height);

	// deprecated
	void SetLSQuads(std::vector<CVSystem::MyQuad> lsQuads) ;

	// set triangle mesh data
	void SetTriangles(std::vector<int> offsets,
					  std::vector<CVSystem::MyPoint> vertexList,
					  std::vector<CVSystem::MyIndexedTriangle> indexedTriangles,
					  std::vector<CVSystem::MyIndexedTriangle> borderSCTriangles,
					  std::vector<CVSystem::MyIndexedTriangle> borderWTriangles,
					  std::vector<CVSystem::MyIndexedTriangle> scTriangles,
					  std::vector<CVSystem::MyIndexedTriangle> wTriangles,
					  std::vector<CVSystem::MyIndexedTriangle> bTriangles);
	// set bezier curve data
	void SetIndexedBezierCurves(std::vector<CVSystem::MyIndexedBezierCurves> indexedBezierCurves);

protected:
	// qt event
	bool event( QEvent * event );
	// init opengl
	void initializeGL();	
	// draw
	void paintGL();

private:
	void DrawDebug();

	// draw mesh without gpu acceleration (no bezier curve, no screentone)
	void DrawMeshWithoutGPU();
	// draw mesh with gpu acceleration 
	void DrawMeshWithGPU();
	// draw screentone with lighting effect
	void DrawLightingEffect();

public:
	// deprecated
	void SetOTriangles(std::vector<CVSystem::MyTriangle> oTriangles)   { this->_oTriangles = oTriangles; }
	// deprecated
	void SetLSTriangles(std::vector<CVSystem::MyTriangle> lsTriangles) { this->_lsTriangles = lsTriangles; }
	// deprecated
	std::vector<CVSystem::MyQuad> lsQuads;
};

#endif
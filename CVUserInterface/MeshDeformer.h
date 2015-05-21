
/**
 * Interface of Mesh deformation using BBW
 * This class is used for creating user interface for deformation,
 * displaying handles and pseudo-edges,
 * etc.
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __Mesh_Deformer__
#define __Mesh_Deformer__

#include <vector>

#include "MyPoint.h"
#include "MyIndexedTriangle.h"
#include "MyIndexedBezierCurves.h"
#include "TriangleType.h"

#include "KDTreeDeclarations.h"
#include "PseudoEdges.h"
#include "WhatDo.h"

// forward declaration
class RenderUtility;
class ScreentoneRenderer;
class CurveRenderer2;
class LightingEffect;

// forward declaration
namespace CVSystem
{
	class PointInTrianglesTest;
}

class MeshDeformer
{
public:
	// Constructor
	MeshDeformer();

	// Destructor
	~MeshDeformer();

	// mouse press
	void mousePressEvent(int x, int y);
	// mouse move
	void mouseMoveEvent(int x, int y);
	// mouse release
	void mouseReleaseEvent(int x, int y);
	// shift button pressed / not pressed
	void SetShiftButton(bool val){_shiftDown = val;}

	// draw (full gpu support)
	void paintGL();	
	// draw (no gpu)
	void paintGLWithoutGPU();	

	// set rendering parameters (for opengl commands)
	void SetVariables(int _x_offset, int _y_offset, int _frame_width, int _frame_height, double _zoom_factor);
	
	// Todo: combine this function with SetTriangles()
	void SetIndexedBezierCurves(std::vector<CVSystem::MyIndexedBezierCurves> indexedBezierCurves);

	void SetTriangles(std::vector<CVSystem::MyPoint>		     vertexList,
					  std::vector<CVSystem::MyIndexedTriangle>   indexedTriangles,
					  std::vector<CVSystem::MyIndexedTriangle>   borderSCTriangles,
					  std::vector<CVSystem::MyIndexedTriangle>   borderWTriangles,
					  std::vector<CVSystem::MyIndexedTriangle>   scTriangles,
					  std::vector<CVSystem::MyIndexedTriangle>   wTriangles,
					  std::vector<CVSystem::MyIndexedTriangle>   bTriangles);
		
	// are handles placed ?
	bool BindReady();	
	// calculate BBW
	void BindHandles();
	// delete handles (press backspace)
	void DeleteHandles();

	// deform a handle to (x,y)
	void DeformMesh(double x, double y);
	// deform an edge
	void DeformPseudoEdge(double x, double y);
	// this function isn't used
	void DeformPseudoEdge2(double x, double y);
	// translation only (no scaling or rotation)
	void DeformTranslation(double x, double y);

	// add a pseudo edge
	void AddPseudoEdge(double x, double y);
	// initialize screentone renderer
	void SetScreentoneSystem(std::vector<double> params, int img_width, int img_height);

	// get instance of lighting effect class
	LightingEffect* GetLightingEffect()							   { return this->_lightingEffect; };
	// get instance of screentone renderer class
	ScreentoneRenderer* GetScreentoneRenderer()					   { return _scRenderer; }
	// get vertex list
	std::vector<CVSystem::MyPoint> GetVertexList()				   { return _vertexList; }
	// get indexed triangle mesh
	std::vector<CVSystem::MyIndexedTriangle> GetIndexedTriangles() { return _indexedTriangles; }

private:
	// render things implemented in opengl
	RenderUtility*					 _renderUtility;
	// render screentone
	ScreentoneRenderer*				 _scRenderer;
	// render bezier curve
	CurveRenderer2*					 _cRendererV2;
	// intersection in between a point and triangle mesh
	CVSystem::PointInTrianglesTest*  _ptInTriangleTest;
	// class to produce gradation mask
	LightingEffect*					 _lightingEffect;

	//// rendering parameters
	int    _x_offset;		// x scroll offset
	int    _y_offset;		// y scroll offset
	int    _frame_width;	// display width
	int    _frame_height;	// display height
	double _zoom_factor;	// zoom (mouse scroll)

	WhatDo _whatDo;			// check before or after BBW calculation
	bool   _mouseDown;		// is mouse pressed
	bool   _shiftDown;		// is shift pressed
	bool   _isDeforming;	// is currently doing deformation (mouse drag)

	Eigen::MatrixXd _weights;			// BBW

	double			 _maxHandleDist;	// maximum distance to select a handle

	// Building PseudoEdge
	PseudoEdges*				_pseudoEdges;

	std::vector<CVSystem::MyPoint> _stroke;		// user's stroke to create pseudo edge
	
	// KD Tree
	PointCloud<float> _vertexPointCloud;
	PointKDTree*	  _vertexKDTree;

	std::vector<CVSystem::MyPoint>				 _vertexList;			// The actual vertices
	std::vector<CVSystem::MyIndexedTriangle>     _indexedTriangles;		// All triangles
	std::vector<CVSystem::MyIndexedTriangle>     _borderSCTriangles;	// Additional screentone triangles
	std::vector<CVSystem::MyIndexedTriangle>     _borderWTriangles;		// Additional white triangles
	std::vector<CVSystem::MyIndexedTriangle>     _scTriangles;			// Screentone triangles
	std::vector<CVSystem::MyIndexedTriangle>     _wTriangles;			// White triangles
	std::vector<CVSystem::MyIndexedTriangle>     _bTriangles;			// Black triangles
	std::vector<CVSystem::MyIndexedBezierCurves> _indexedBezierCurves;	// Bezier curves (start and end points only)

private:
	//// draw things
	void DrawPseudoEdges();
	void DrawStrokes();
	void DrawHandles();
	void DrawMeshWithGPU();
	void DrawMeshWithoutGPU();	

	void BuildVertexKDTree();

	// add a handle in (x, y)
	CVSystem::MyPoint AddHandle(float x, float y);	
	//get index1 and index2
	void GetIndices(float x, float y, int& index1, int& index2);

	// query vertex
	void QueryVertexPoint(float x, float y, CVSystem::MyPoint& pt, int& index1);
	// query handle
	void QueryHandlePoint(float x, float y, CVSystem::MyPoint& pt, int& index1, int& index2);
};

#endif

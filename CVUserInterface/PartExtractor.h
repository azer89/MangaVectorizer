
/**
 * Class used for contextual component completion
 *
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#ifndef __Part_Extractor__
#define __Part_Extractor__

#include <vector>
#include "MyPoint.h"
#include "MyIndexedBezierCurves.h"
#include "MyIndexedTriangle.h"
#include "MyIndexedLine.h"
#include "MyIndexedPolygon.h"

#include "UtilityFunctions.h"
#include "KDTreeDeclarations.h"

#include "PointLocationTest2.h"

#include "IUI_StrokeType.h"

// forward declaration
namespace CVSystem { class PointInTrianglesTest; }
class ColorCurveRenderer;
class CurveSorter;


class PartExtractor
{
public:
	// Constructor
	PartExtractor();

	// Destructor
	~PartExtractor();

	//// mouse events
	// press
	void mousePressEvent(int x, int y);
	// move
	void mouseMoveEvent( int x, int y);
	// release
	void mouseReleaseEvent(int x, int y);

	// draw
	void paintGL();

	// set data
	void SetData(std::vector<CVSystem::MyPoint>				  vertexList, 
				 std::vector<CVSystem::MyIndexedBezierCurves> indexedBezierCurves,
				 std::vector<CVSystem::MyIndexedTriangle>	  blackTriangles);
		
	// do registration of user stroke and original borders
	void DoNonRigidCorrespondence();
	// get nearby lines
	void GetCandidateLines();
	// extracting parts
	void ExtractPart();
	// undo all
	void DoUndo();

	// extracting parts (point location)
	void DoPointLocationTest(double x, double y);
	// convert to polygon
	void MakePart(std::vector<std::vector<int>>	idxBoundaries);
	// add to list
	void AppendPart(std::vector<std::vector<int>> idxBoundaries);

	// init strokes
	void InitStrokeType();
	// swap stroke type (isn't implemented yet)
	void SwapStrokeType();


	// unused
	bool HasData() { return _hasData;}
	// unused
	void SetShiftButtonState(bool val) { _shiftDown = val; }

	// single parts
	std::vector<CVSystem::MyIndexedPolygon> GetParts();
	// partition lines only
	std::vector<CVSystem::MyIndexedLine> GetPartitionLines();
	// currently isn't implemented
	std::vector<IUI_StrokeType>	GetPartitionLineTypes();


private:
	// line data structure
	void CreateLineKDTree();
	// point data structure
	void CreatePointKDTree();

	// remove far scribble points
	void   RemoveFarPoints(std::vector<CVSystem::MyPoint>& points);
	// is partition line valid
	bool   CheckLineValidity(CVSystem::MyLine line);
	// Is point near border
	bool   IsNearBorder(CVSystem::MyPoint curPt);
	// Distance a point to closest border
	double PointDistanceToBorder(CVSystem::MyPoint curPt);

	
	// does a point lie exactly at any vertex
	bool IsOriginalPoint(CVSystem::MyPoint pt);
	// get closest point index
	int	 GetClosestPointIndex(CVSystem::MyPoint pt);

	//// Draw functions
	// draw original strokes
	void DrawOriginalStrokes();
	// draw sampled points (far points are removed)
	void DrawSimpleStrokes();
	// this function isn't used
	void DrawSelectedPoints();
	// draw partition lines
	void DrawPartitionLines();
	// draw sample points (for debugging)
	void DrawSamplePoints();
	// draw point-to-point correspondence (for debugging)
	void DrawCorrespondenceLines();
	// draw parts
	void DrawParts();
	// draw simple a quad
	void DrawQuad(CVSystem::MyPoint pt, double halfWidth, double red, double green, double blue);

private:

	bool	_hasData;				// true after calling SetData()
	bool	_mouseDown;				// is mouse down
	bool	_shiftDown;				// is shift down
	double	_neg_curvature_param;	// not used

	IUI_StrokeType	_currentStrokeType;	// isn't implemented yet
	QRgb		_strokeColors[2];		// stroke color

	LineCloud<float> _lineCloud;	// line data
	LineKDTree*		 _lineKDTree;	// line kd-tree
	
	PointCloud<float> _pointCloud;	// point data
	PointKDTree* 	  _pointKDTree;	// point kd tree

	CVSystem::PointInTrianglesTest* _ptInTrianglesTest;	// point with triangle intersection test
	CVSystem::PointLocationTest2*	_plTest;			// point location algorithm
	ColorCurveRenderer*				_ccRenderer;		// line renderer
	CurveSorter*					_cSorter;			// sorter to obtain closed curve
		
	// Users input	
	std::vector<CVSystem::MyPoint>				 _oriStroke;			// original noisy users stroke
	std::vector<std::vector<CVSystem::MyPoint>>  _sampledStrokes;		// sampled users stroke
	std::vector<IUI_StrokeType>					 _sampledStrokesTypes;	// isn't implemented yet

	// Candidate
	std::vector<CVSystem::MyPoint>	_uniformCdtPoints;		// uniform sampling from bezier curves

	// Non-rigid registration
	std::vector<CVSystem::MyIndexedLine> _idxPartitionLines;		// line partition
	std::vector<IUI_StrokeType>			 _idxPartitionLineTypes;	// isn't implemented yet

	// Base data
	std::vector<CVSystem::MyPoint>				 _vertexList;	// storing each vertex position
	std::vector<CVSystem::MyIndexedBezierCurves> _idxBzCurves;	// original bezier curves
	std::vector<CVSystem::MyIndexedTriangle>	 _bTriangles;	// black triangles

	// Display debug (not important at all)
	std::vector<CVSystem::MyLine> _correspondLines;	// displaying point correspondences
	
	CVSystem::MyIndexedPolygon				_currentPart;
	std::vector<CVSystem::MyIndexedPolygon>	_parts;
};

#endif

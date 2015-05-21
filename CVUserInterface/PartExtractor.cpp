
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "PartExtractor.h"
#include "LinesSorter.h"
#include "NonRigidPointMatch.h"
#include "SystemParams.h"
#include "PointInTrianglesTest.h"
#include "ColorCurveRenderer.h"

/** 
 * Constructor
 */
PartExtractor::PartExtractor() : 	 
	_hasData(false),
	_neg_curvature_param(-(M_PI / 12.0)),
	_ptInTrianglesTest(0),
	_plTest(0),
	_ccRenderer(0),
	_mouseDown(false),
	_shiftDown(false)
{
}

/** 
 * Destructor
 */
PartExtractor::~PartExtractor()
{
	if(_ptInTrianglesTest) { delete _ptInTrianglesTest; }
	if(_ccRenderer)		   { delete _ccRenderer; }
}

/** 
 *
 */
void PartExtractor::mousePressEvent(int x, int y)
{
	_mouseDown = true;
}

/** 
 *
 */
void PartExtractor::mouseMoveEvent(int x, int y)
{
	if(!_mouseDown) { return; }
	_oriStroke.push_back(CVSystem::MyPoint(x, y));
}

/** 
 *
 */
void PartExtractor::mouseReleaseEvent(int x, int y)
{
	_mouseDown = false;

	//// Create uniform curve from stroke	
	if(_oriStroke.size() <= 5) { _oriStroke.clear(); return; }
	std::vector<CVSystem::MyPoint> simpleStroke;
	UtilityFunctions::UniformResample(_oriStroke, simpleStroke, 
									  UtilityFunctions::CurveLength(_oriStroke) / SystemParams::pe_sample_distance);
	RemoveFarPoints(simpleStroke);
		
	if(simpleStroke.size() == 0) { return; }
	
	_sampledStrokes.push_back(simpleStroke);
	_sampledStrokesTypes.push_back(_currentStrokeType);

	_oriStroke.clear();

	//// do matching
	GetCandidateLines();
}

/** 
 *
 */
void PartExtractor::RemoveFarPoints(std::vector<CVSystem::MyPoint>& points)
{
	std::vector<CVSystem::MyPoint> newPoints;
	double searchRadius = SystemParams::pe_search_radius;

	for(size_t a = 0; a < points.size(); a++)
	{
		float query_pt[2] = { points[a].x, points[a].y};
		float radius = searchRadius * 2.0;
		std::vector<std::pair<size_t,float>> ret_matches;
		nanoflann::SearchParams params;
		size_t nMatches = _lineKDTree->radiusSearch(&query_pt[0],radius, ret_matches, params);

		if(nMatches > 0) { newPoints.push_back(points[a]); }
	}

	points.clear();
	for(size_t a = 0; a < newPoints.size(); a++)
		{ points.push_back(newPoints[a]); }
}

/** 
 *
 */
void PartExtractor::GetCandidateLines()
{
	using namespace CVSystem;

	std::vector<CVSystem::MyLine>			cdtLines;
	std::vector<CVSystem::MyIndexedLine>	idxCdtLines;

	_uniformCdtPoints.clear();
		
	double searchRadius = SystemParams::pe_search_radius;

	for(size_t a = 0; a < _sampledStrokes.size(); a++)
	{
		std::vector<MyPoint> simpleStroke = _sampledStrokes[a];
		for(size_t b = 1; b < simpleStroke.size(); b++)	////
		{
			MyPoint curPt = simpleStroke[b];
			float query_pt[2] = { simpleStroke[b].x, simpleStroke[b].y};
			float radius = searchRadius + searchRadius * 0.25;

			std::vector<std::pair<size_t,float>> ret_matches;
			nanoflann::SearchParams params;
			size_t nMatches = _lineKDTree->radiusSearch(&query_pt[0],radius, ret_matches, params);
			for (size_t c = 0; c < nMatches; c++)
			{
				int idx = ret_matches[c].first;
				MyPoint ptA = _vertexList[_lineCloud.lines[idx].index0]; 
				MyPoint ptB = _vertexList[_lineCloud.lines[idx].index1];
				
				////
				MyPoint curDir = ( simpleStroke[b] - simpleStroke[b-1] ).Norm();
				MyPoint sampleDir = ( ptB - ptA ).Norm();
				MyPoint sampleDirRev = ( ptA - ptB ).Norm();
				double angleRad1 = std::abs(UtilityFunctions::GetRotation(curDir, sampleDir));
				double angleRad2 = std::abs(UtilityFunctions::GetRotation(curDir, sampleDirRev));
				double angleRad = std::min(angleRad1, angleRad2);

				////
				if(UtilityFunctions::DistanceToFiniteLine(ptA, ptB, curPt) < searchRadius && angleRad < (M_PI / 5.0))	//
				{ 
					cdtLines.push_back(MyLine(ptA, ptB));
					idxCdtLines.push_back(MyIndexedLine(_lineCloud.lines[idx].index0, _lineCloud.lines[idx].index1));
				}
			}
		}
	}

	std::cout << "\n";

	//// mark all indices as false
	std::vector<bool> flagList(_vertexList.size());
	for(size_t a = 0; a < idxCdtLines.size(); a++)
	{ flagList[idxCdtLines[a].index0] = false; flagList[idxCdtLines[a].index1] = false; }

	for(size_t a = 0; a < cdtLines.size(); a++)
	{
		MyLine curLine = cdtLines[a];
		MyIndexedLine curIndexedLine = idxCdtLines[a];
		int nSegment = curLine.Magnitude() / SystemParams::pe_sample_distance;
		std::vector<CVSystem::MyPoint> resampleCurves;

		if(nSegment <= 1) {nSegment = 2;}
		UtilityFunctions::UniformResample(curLine, resampleCurves, nSegment);

		/*if(nSegment <= 1 )
		{ resampleCurves.push_back(MyPoint(curLine.XA, curLine.YA)); resampleCurves.push_back(MyPoint(curLine.XB, curLine.YB)); }
		else
		{ UtilityFunctions::UniformResample(curLine, resampleCurves, nSegment); }*/

		for(size_t b = 0; b < resampleCurves.size(); b++)
		{
			if(b == 0 && !flagList[curIndexedLine.index0])
			{
				_uniformCdtPoints.push_back(resampleCurves[b]);
				flagList[curIndexedLine.index0] = true;
			}
			else if(b == resampleCurves.size() - 1 && !flagList[curIndexedLine.index1])
			{
				_uniformCdtPoints.push_back(resampleCurves[b]);
				flagList[curIndexedLine.index1] = true;
			}
			else
			{ _uniformCdtPoints.push_back(resampleCurves[b]); }
		}
	}
}

/** 
 *
 */
void PartExtractor::SetData(std::vector<CVSystem::MyPoint>				 vertexList, 
							std::vector<CVSystem::MyIndexedBezierCurves> indexedBezierCurves,
							std::vector<CVSystem::MyIndexedTriangle>	 bTriangles)
{
	//// Assign to vectors
	this->_vertexList		   = vertexList;
	this->_idxBzCurves = indexedBezierCurves;
	this->_bTriangles		   = bTriangles;

	if(_ccRenderer) {delete _ccRenderer;}
	_ccRenderer = new ColorCurveRenderer();
	_ccRenderer->SetData(_vertexList, _idxBzCurves);

	_ptInTrianglesTest = new CVSystem::PointInTrianglesTest();
	_ptInTrianglesTest->PushTriangles(_vertexList, _bTriangles);

	//// Create line kd-tree
	CreateLineKDTree();

	//// Corners
	//CalculateCorners();

	//// Create point kd-tree
	CreatePointKDTree();

	//// Init
	//InitCounter();
	InitStrokeType();

	//// Set flag
	_hasData = true;

	_plTest = new CVSystem::PointLocationTest2();
	std::vector<CVSystem::MyLine> lines;

	for(size_t a = 0; a < _idxBzCurves.size(); a++)
	{
		CVSystem::MyIndexedBezierCurves curve = _idxBzCurves[a];
		for(size_t b = 0; b < curve.indices.size() - 1; b++)
			{ lines.push_back(CVSystem::MyLine(_vertexList[curve.indices[b]], _vertexList[curve.indices[b+1]])); }
	}

	_plTest->SetShapeSegments(lines);
}

void PartExtractor::InitStrokeType()
{
	_currentStrokeType = IUI_STROKE_FOREGROUND;
	_strokeColors[0] = qRgb(255, 0, 0);
	_strokeColors[1] = qRgb(0,   174, 239);
}

void PartExtractor::SwapStrokeType()
{
	_currentStrokeType = IUI_StrokeType(1 - _currentStrokeType);
}

/** 
 *
 */
void PartExtractor::CreatePointKDTree()
{
	using namespace CVSystem;

	//// how many point?
	int numPt = 0;
	for(size_t a = 0; a < _idxBzCurves.size(); a++)
	{ numPt += _idxBzCurves[a].indices.size() - 1; }

	_pointCloud.pts.resize(numPt);
	int nCounter = 0;
	for(size_t a = 0; a < _idxBzCurves.size(); a++)
	{
		MyIndexedBezierCurves bCurve = _idxBzCurves[a];
		for(size_t b = 0; b < bCurve.indices.size() - 1; b++)
		{
			MyPoint pt = _vertexList[bCurve.indices[b]];
			_pointCloud.pts[nCounter].x = pt.x; 
			_pointCloud.pts[nCounter].y = pt.y;
			_pointCloud.pts[nCounter].info1 = bCurve.indices[b];
			
			//// corner flags seem useless
			//if(bCurve.cornerFlags[b]) { _pointCloud.pts[nCounter].info1 = 1; }
			//else { _pointCloud.pts[nCounter].info1 = 0; }

			nCounter++;
		}
	}

	//// Create point tree
	_pointKDTree = new PointKDTree(2 /*dim*/, _pointCloud, KDTreeSingleIndexAdaptorParams(64 /* max leaf */) );
	_pointKDTree->buildIndex();
}

/** 
 *
 */
void PartExtractor::CreateLineKDTree()
{
	using namespace CVSystem;

	//// how many line segment?
	int numLine = 0;
	for(size_t a = 0; a < _idxBzCurves.size(); a++)
	{ numLine += _idxBzCurves[a].indices.size() - 1; }

	//// Feed to line cloud with mid points
	_lineCloud.lines.resize(numLine);
	int nCounter = 0;
	for(size_t a = 0; a < _idxBzCurves.size(); a++)
	{
		MyIndexedBezierCurves bCurve = _idxBzCurves[a];
		for(size_t b = 1; b < bCurve.indices.size(); b++)
		{
			MyPoint ptA = _vertexList[bCurve.indices[b - 1]]; MyPoint ptB = _vertexList[bCurve.indices[b]];
			MyPoint ptC = (ptA + ptB) / 2.0;

			_lineCloud.lines[nCounter].x = ptC.x; _lineCloud.lines[nCounter].y = ptC.y;
			_lineCloud.lines[nCounter].index0 = bCurve.indices[b - 1]; _lineCloud.lines[nCounter].index1 = bCurve.indices[b];
			nCounter++;
		}
	}

	//// Create line tree
	_lineKDTree = new LineKDTree(2 /*dim*/, _lineCloud, KDTreeSingleIndexAdaptorParams(64 /* max leaf */) );
	_lineKDTree->buildIndex();
}

void PartExtractor::DoPointLocationTest(double x, double y)
{
	using namespace CVSystem;

	std::cout << "DoPartExtraction(" << x << ", " << y << ")\n";

	std::vector<MyLine> lines;
	std::vector<MyLine> cutLines;
	
	for(size_t a = 0; a < _idxBzCurves.size(); a++)
	{
		MyIndexedBezierCurves curve = _idxBzCurves[a];
		for(size_t b = 0; b < curve.indices.size() - 1; b++)
			{ lines.push_back(MyLine(_vertexList[curve.indices[b]], _vertexList[curve.indices[b+1]])); }
	}

	for(size_t a = 0; a < _idxPartitionLines.size(); a++)
	{
		MyPoint pt1 = _vertexList[_idxPartitionLines[a].index0];
		MyPoint pt2 = _vertexList[_idxPartitionLines[a].index1];
		cutLines.push_back(MyLine(pt1, pt2));
	}

	std::vector<std::vector<CVSystem::MyPoint>>	boundaries;
	//boundaries = PointLocationTest1::GetEnclosingPolygons(lines, cutLines, MyPoint(x, y));
	boundaries = _plTest->GetEnclosingPolygons(cutLines, MyPoint(x, y));

	std::vector<std::vector<int>>	idxBoundaries;

	/// Snap
	for(size_t a = 0; a < boundaries.size(); a++)
	{
		std::vector<int> idxBoundary;
		for(size_t b = 0; b < boundaries[a].size(); b++)
		{
			int idx = GetClosestPointIndex(boundaries[a][b]);
			idxBoundary.push_back(idx);
		}
		idxBoundaries.push_back(idxBoundary);
	}

	if(_shiftDown && _currentPart.boundary.size() > 0)
		{ AppendPart(idxBoundaries); }
	else
		{ MakePart(idxBoundaries); }
}

void PartExtractor::MakePart(std::vector<std::vector<int>>	idxBoundaries)
{
	using namespace CVSystem;

	//// Convert to polygon
	_currentPart.boundary.clear();
	_currentPart.holes.clear();
	for(size_t a = 0; a < idxBoundaries.size(); a++)
	{
		std::vector<int> idxBoundary = idxBoundaries[a];

		if(a == 0)	
		{
			//// Outer boundary
			if(UtilityFunctions::IsClockwise(idxBoundary, _vertexList))
			{ std::reverse(idxBoundary.begin(), idxBoundary.end()); }
			std::vector<MyIndexedLine> lines = UtilityFunctions::IndexedPointsToLine(idxBoundary);
			_currentPart.boundary = lines;
		}
		else
		{
			//// Hole
			if(!UtilityFunctions::IsClockwise(idxBoundary, _vertexList))
			{ std::reverse(idxBoundary.begin(), idxBoundary.end()); }
			std::vector<MyIndexedLine> lines = UtilityFunctions::IndexedPointsToLine(idxBoundary);
			_currentPart.holes.push_back(lines);
		}
	}
}

void PartExtractor::AppendPart(std::vector<std::vector<int>> idxBoundaries)
{
	using namespace CVSystem;

	for(size_t a = 0; a < idxBoundaries.size(); a++)
	{
		std::vector<int> idxBoundary = idxBoundaries[a];

		if(a == 0)	
		{
			//// Outer boundary
			if(UtilityFunctions::IsClockwise(idxBoundary, _vertexList))
			{ std::reverse(idxBoundary.begin(), idxBoundary.end()); }
			std::vector<MyIndexedLine> lines = UtilityFunctions::IndexedPointsToLine(idxBoundary);
			
			for(size_t b = 0; b < lines.size(); b++)
			{
				if(!UtilityFunctions::DoesExist(_currentPart.boundary, lines[b]))
				{
					_currentPart.boundary.push_back(lines[b]);
				}
			}
			
			//_part.boundary = lines;
		}
		else
		{
			//// Hole
			if(!UtilityFunctions::IsClockwise(idxBoundary, _vertexList))
			{ std::reverse(idxBoundary.begin(), idxBoundary.end()); }
			std::vector<MyIndexedLine> lines = UtilityFunctions::IndexedPointsToLine(idxBoundary);
			_currentPart.holes.push_back(lines);
		}
	}
}

/** 
 *
 */
void PartExtractor::DoNonRigidCorrespondence()
{
	_correspondLines.clear();
	_idxPartitionLines.clear();

	using namespace CVSystem;
	std::vector<MyPoint> tempPoints;
	std::vector<int> indices1;
	std::vector<int> indices2;
	for(size_t a = 0; a < _sampledStrokes.size(); a++)
	{
		std::vector<MyPoint> simpleStroke = _sampledStrokes[a];
		for(size_t b = 0; b < simpleStroke.size(); b++)
		{
			tempPoints.push_back(simpleStroke[b]);
			indices1.push_back(a);
			indices2.push_back(b);
		}
	}

	NonRigidPointMatch::DoPointMatch(tempPoints, _uniformCdtPoints, _correspondLines);

	for(size_t a = 0; a < tempPoints.size(); a++)
		{ _sampledStrokes[indices1[a]][indices2[a]] = tempPoints[a]; }

	std::vector<std::vector<MyPoint>> matchedPoints;
	for(size_t a = 0; a < _sampledStrokes.size(); a++)
	{
		std::vector<MyPoint> pts;
		matchedPoints.push_back(pts);
	}

	for(size_t a = 0; a < _correspondLines.size(); a++)
	{
		MyPoint pt(_correspondLines[a].XB, _correspondLines[a].YB);
		{ matchedPoints[indices1[a]].push_back(pt); } 
	}

	std::vector<CVSystem::MyLine> selectedLines;
	for(size_t a = 0; a < matchedPoints.size(); a++)
	{
		std::vector<MyPoint> points = matchedPoints[a];
		std::vector<MyLine> lines;
		for(size_t b = 1; b < points.size(); b++)
		{
			MyLine line(points[b-1], points[b]);
			if(CheckLineValidity(line))
			{ 
				selectedLines.push_back(line); 
				_idxPartitionLines.push_back(MyIndexedLine(-1, -1));
				_idxPartitionLineTypes.push_back(_sampledStrokesTypes[a]);
			}
		}
	}

	//// Snap to the closest point
	for(size_t a = 0; a < selectedLines.size(); a++)
	{
		int idx1 = GetClosestPointIndex(MyPoint(selectedLines[a].XA, selectedLines[a].YA));
		int idx2 = GetClosestPointIndex(MyPoint(selectedLines[a].XB, selectedLines[a].YB));

		_idxPartitionLines[a].index0 = idx1;
		_idxPartitionLines[a].index1 = idx2;
	}
}

/** 
 *
 */
void PartExtractor::ExtractPart()
{
	this->_parts.push_back(_currentPart);
}

std::vector<CVSystem::MyIndexedLine> PartExtractor::GetPartitionLines()
{
	return _idxPartitionLines;
}

std::vector<IUI_StrokeType>	PartExtractor::GetPartitionLineTypes()
{
	return _idxPartitionLineTypes;
}

std::vector<CVSystem::MyIndexedPolygon> PartExtractor::GetParts() 
{ 
	using namespace CVSystem;

	std::vector<bool> flags;
	for(size_t a = 0; a < _vertexList.size(); a++)
		{ flags.push_back(false); }

	for(size_t a = 0; a < _parts.size(); a++)
	{
		MyIndexedPolygon* poly = & _parts[a];

		//// distinct points
		std::vector<MyPoint> pts;
		std::vector<int>	 indices;

		//// Boundary
		for(size_t b = 0; b < poly->boundary.size(); b++)
		{
			if(!flags[poly->boundary[b].index0])
			{
				pts.push_back(_vertexList[poly->boundary[b].index0]);
				indices.push_back( poly->boundary[b].index0);
				flags[poly->boundary[b].index0] = true;
			}

			if(!flags[poly->boundary[b].index1])
			{
				pts.push_back(_vertexList[poly->boundary[b].index1]);
				indices.push_back(poly->boundary[b].index1);
				flags[poly->boundary[b].index1] = true;
			}
		}

		//// Holes
		for(size_t b = 0; b < poly->holes.size(); b++)
		{
			std::vector<MyIndexedLine> hole = poly->holes[b];
			for(size_t c = 0; c < hole.size(); c++)
			{
				if(!flags[hole[c].index0])
				{
					pts.push_back(    _vertexList[hole[c].index0]);
					indices.push_back(            hole[c].index0);
					flags[hole[c].index0] = true;
				}

				if(!flags[hole[c].index1])
				{
					pts.push_back(    _vertexList[hole[c].index1]);
					indices.push_back(            hole[c].index1);
					flags[hole[c].index1] = true;
				}
			}
		}

		poly->distinctPoints = pts;
		poly->distinctIndices = indices;
	}



	return _parts; 
	
}

/** 
 *
 */
bool PartExtractor::CheckLineValidity(CVSystem::MyLine line)
{
	using namespace CVSystem;
	
	MyPoint startPoint(line.XA, line.YA);
	MyPoint midPoint( (line.XA + line.XB) / 2.0, (line.YA + line.YB) / 2.0 );
	MyPoint endPoint(line.XB, line.YB);

	//bool sFlag = IsOnBorder(startPoint);
	//bool mbFlag = IsNearBorder(midPoint);
	//bool eFlag = IsOnBorder(endPoint);

	bool mtFlag = _ptInTrianglesTest->IsInside(midPoint);
	double dist = PointDistanceToBorder(midPoint);

	if(/*mbFlag ||*/ !mtFlag  || dist < SystemParams::pe_threshold ) 
		{ return false; }

	return true;
}

/** 
 *
 */
bool PartExtractor::IsNearBorder(CVSystem::MyPoint curPt)
{
	using namespace CVSystem;

	float query_pt[2] = { curPt.x, curPt.y};
	double searchRadius = SystemParams::pe_search_radius;	
	float radius = searchRadius + searchRadius * 0.5;

	std::vector<std::pair<size_t,float>> ret_matches;
	nanoflann::SearchParams params;
	size_t nMatches = _lineKDTree->radiusSearch(&query_pt[0],radius, ret_matches, params);
	for (size_t c = 0; c < nMatches; c++)
	{
		int idx = ret_matches[c].first;
		MyPoint ptA = _vertexList[_lineCloud.lines[idx].index0]; 
		MyPoint ptB = _vertexList[_lineCloud.lines[idx].index1];
		if(UtilityFunctions::DistanceToFiniteLine(ptA, ptB, curPt) < SystemParams::pe_threshold)
			{ return true; }
	}
	return false;
}

/** 
 *
 */
double PartExtractor::PointDistanceToBorder(CVSystem::MyPoint curPt)
{
	using namespace CVSystem;

	float  query_pt[2] = { curPt.x, curPt.y};
	double searchRadius = SystemParams::pe_search_radius;	
	float  radius = searchRadius * 10.0;
	double threshold = SystemParams::pe_threshold;

	double minDist = DBL_MAX;

	std::vector<std::pair<size_t,float>> ret_matches;
	nanoflann::SearchParams params;
	size_t nMatches = _lineKDTree->radiusSearch(&query_pt[0],radius, ret_matches, params);
	for (size_t c = 0; c < nMatches; c++)
	{
		int idx = ret_matches[c].first;
		MyPoint ptA = _vertexList[_lineCloud.lines[idx].index0]; 
		MyPoint ptB = _vertexList[_lineCloud.lines[idx].index1];

		double d = UtilityFunctions::DistanceToFiniteLine(ptA, ptB, curPt);
		if(d < minDist) { minDist = d; }
	}
	return minDist;
}

/** 
 *
 */
void PartExtractor::paintGL()
{
	using namespace CVSystem;

	//if(_ccRenderer) {_ccRenderer->paintGL();}

	//// Draw original strokes
	DrawOriginalStrokes();

	//// Draw simple strokes
	DrawSimpleStrokes();

	//// Draw selected points from non rigid registration
	//DrawSelectedPoints();

	//// Selected lines
	DrawPartitionLines();

	//// Correspondence lines
	//DrawCorrespondenceLines();

	//// Draw sample points
	//DrawSamplePoints();

	DrawParts();

	//// Draw corners
	//for(size_t a = 0; a < _cornerPoints.size(); a++)
	//{ DrawQuad(_cornerPoints[a], 0.5, 1, 1, 0); }
}

/** 
 *
 */
void PartExtractor::DrawOriginalStrokes()
{
	using namespace CVSystem;
	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(3.0);
	glBegin(GL_LINES);
	if(_oriStroke.size() > 1) // Original users' stroke
	{
		QColor col(_strokeColors[_currentStrokeType]);
		glColor4f(col.red() / 255.0, col.green() / 255.0, col.blue() / 255.0, 1.0);

		for(size_t a = 0; a < _oriStroke.size() -1; a++)
		{ glVertex3f(_oriStroke[a].x, _oriStroke[a].y, 0); glVertex3f(_oriStroke[a+1].x, _oriStroke[a+1].y, 0); }
	}	
	glEnd();

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

/** 
 *
 */
void PartExtractor::DrawSimpleStrokes()
{
	using namespace CVSystem;
	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(3.0);
	glBegin(GL_LINES);
	if(_sampledStrokes.size() > 0 && _idxPartitionLines.size() == 0) // Simplified users' stroke
	{
		for(size_t a = 0; a < _sampledStrokes.size(); a++)
		{
			QColor col(_strokeColors[_sampledStrokesTypes[a]]);
			glColor4f(col.red() / 255.0, col.green() / 255.0, col.blue() / 255.0, 1.0);

			std::vector<MyPoint> simpleStroke = _sampledStrokes[a];
			for(size_t b = 1; b < simpleStroke.size(); b++)
			{
				
				glVertex3f(simpleStroke[b-1].x, simpleStroke[b-1].y, 0);  
				glVertex3f(simpleStroke[b].x,   simpleStroke[b].y, 0);
			}
		}
	}
	glEnd();

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

/** 
 *
 */
void PartExtractor::DrawSelectedPoints()
{
	/*using namespace CVSystem;
	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(3.0);
	glBegin(GL_LINES);
	glColor4f(1.0, 1.0, 0.0, 1.0);
	for(size_t a = 0; a < _selectedLines.size(); a++)
	{
		QColor col(_colors[_counterList[a]]);
		glColor4f(col.red() / 255.0, col.green() / 255.0, col.blue() / 255.0, 1.0);

		for(size_t b = 0; b < _selectedLines[a].size(); b++)
		{

			MyLine line = _selectedLines[a][b];
			glVertex3f(line.XA, line.YA, 0); 
			glVertex3f(line.XB, line.YB, 0);

		}
	}
	glEnd();

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);*/
}

/** 
 *
 */
void PartExtractor::DrawPartitionLines()
{
	using namespace CVSystem;
	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(3.0);
	glBegin(GL_LINES);
	glColor4f(1.0, 1.0, 0.0, 1.0);
	for(size_t a = 0; a < _idxPartitionLines.size(); a++)
	{
		QColor col(_strokeColors[_idxPartitionLineTypes[a]]);
		glColor4f(col.red() / 255.0, col.green() / 255.0, col.blue() / 255.0, 1.0);

		MyPoint pt1 = _vertexList[_idxPartitionLines[a].index0];
		MyPoint pt2 = _vertexList[_idxPartitionLines[a].index1];
			
		glVertex3f(pt1.x, pt1.y, 0); 
		glVertex3f(pt2.x, pt2.y, 0);
	}
	glEnd();

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

/** 
 *
 */
void PartExtractor::DrawCorrespondenceLines()
{
	using namespace CVSystem;
	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(3.0);
	glBegin(GL_LINES);
	glColor4f(0.0, 1.0, 0.0, 1.0);
	for(size_t a = 0; a < _correspondLines.size(); a++)
	{
		MyLine cLine = _correspondLines[a];
		glVertex3f(cLine.XA, cLine.YA, 0);	 
		glVertex3f(cLine.XB, cLine.YB, 0);
	}
	glEnd();

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

/** 
 *
 */
void PartExtractor::DrawSamplePoints()
{
	for(size_t a = 0; a < _uniformCdtPoints.size(); a++)
		{ DrawQuad(_uniformCdtPoints[a], 0.5, 0, 0, 1); }
}

void PartExtractor::DrawParts()
{
	using namespace CVSystem;
	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(3.0);
	glBegin(GL_LINES);

	//// Draw outer boundary
	glColor4f(0.0, 1.0, 0.0, 1.0);
	for(size_t a = 0; a < _currentPart.boundary.size(); a++)
	{
		MyPoint ptA = _vertexList[_currentPart.boundary[a].index0];
		MyPoint ptB = _vertexList[_currentPart.boundary[a].index1];
		
		//glColor4f(1.0, 1.0, 1.0, 1.0);
		glVertex3f(ptA.x, ptA.y, 0);	 
		
		//glColor4f(0.0, 1.0, 0.0, 1.0);
		glVertex3f(ptB.x, ptB.y, 0);
	}

	//// Draw holes
	for(size_t a = 0; a < _currentPart.holes.size(); a++)
	{
		std::vector<MyIndexedLine> hole = _currentPart.holes[a];
		for(size_t b = 0; b < hole.size(); b++)
		{
			MyPoint ptA = _vertexList[hole[b].index0];
			MyPoint ptB = _vertexList[hole[b].index1];

			//glColor4f(1.0, 1.0, 1.0, 1.0);
			glVertex3f(ptA.x, ptA.y, 0);	 

			//glColor4f(0.0, 1.0, 0.0, 1.0);
			glVertex3f(ptB.x, ptB.y, 0);
		}
	}

	glEnd();

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

/** 
 *
 */
void PartExtractor::DoUndo()
{
	_oriStroke.clear();
	_sampledStrokes.clear();
	_sampledStrokesTypes.clear();
	_uniformCdtPoints.clear();
	_idxPartitionLines.clear();
	_idxPartitionLineTypes.clear();

	_currentPart.boundary.clear();
	_currentPart.holes.clear();
}

/** 
 *
 */
void PartExtractor::DrawQuad(CVSystem::MyPoint pt, double halfWidth, double red, double green, double blue)
{
	glDepthMask(GL_FALSE);	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);

	glColor4f(red, green, blue, 1.0);
	glVertex3f(pt.x - halfWidth, pt.y - halfWidth, 0);
	glVertex3f(pt.x + halfWidth, pt.y - halfWidth, 0);
	glVertex3f(pt.x + halfWidth, pt.y + halfWidth, 0);
	glVertex3f(pt.x - halfWidth, pt.y + halfWidth, 0);

	glEnd();	// GL_QUADS
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

/** 
* Note that info1 is index
 */
int	PartExtractor::GetClosestPointIndex(CVSystem::MyPoint pt)
{
	float query_pt[2] = { pt.x, pt.y};

	std::vector<size_t> ret_index(1);
	std::vector<float> out_dist_sqr(1);
	_pointKDTree->knnSearch(&query_pt[0], 1, &ret_index[0], &out_dist_sqr[0]);
	double dist = out_dist_sqr[0];
	int retIdx = ret_index[0];

	return _pointCloud.pts[retIdx].info1;
}

/** 
 * Not used
 */
bool PartExtractor::IsOriginalPoint(CVSystem::MyPoint pt)
{
	float query_pt[2] = { pt.x, pt.y};

	std::vector<size_t> ret_index(1);
	std::vector<float> out_dist_sqr(1);
	_pointKDTree->knnSearch(&query_pt[0], 1, &ret_index[0], &out_dist_sqr[0]);
	double dist = out_dist_sqr[0];
	int retIdx = ret_index[0];

	if(dist > -M_EPS && dist < M_EPS)
	{ return true; }

	return false;
}

/** 
 * Not used
 */
//void PartExtractor::CalculateCorners()
//{
//	using namespace CVSystem;
//
//	_cornerPoints.clear();
//
//	//// Create corner flags
//	for(size_t a = 0; a < _indexedBezierCurves.size(); a++)
//	{
//		MyIndexedBezierCurves* bCurve = &_indexedBezierCurves[a];
//
//		//// Init corner flags
//		if(bCurve->cornerFlags.size() != bCurve->indices.size()) 
//		{ bCurve->cornerFlags.resize(bCurve->indices.size()); }
//		for(size_t b = 0; b < bCurve->cornerFlags.size() - 1; b++) 
//		{ bCurve->cornerFlags[b] = 0; }
//	}
//
//	for(size_t a = 0; a < _indexedBezierCurves.size(); a++)
//	{
//		MyIndexedBezierCurves* bCurve = &_indexedBezierCurves[a];
//		for(size_t b = 0; b < bCurve->indices.size() - 1; b++)
//		{
//			int prevIndex = b - 1; int nextIndex = b + 1;
//
//			if(b == bCurve->indices.size() - 2) { nextIndex = 0; }
//			else if(b == 0) { prevIndex = bCurve->indices.size() - 2; }
//
//			MyPoint curPt  = _vertexList[bCurve->indices[b]];
//			MyPoint prevPt = _vertexList[bCurve->indices[prevIndex]]; 
//			MyPoint nextPt = _vertexList[bCurve->indices[nextIndex]];
//
//			MyPoint prevDir = curPt - prevPt; 
//			MyPoint nextDir = nextPt - curPt;
//			prevDir = prevDir.Norm(); nextDir = nextDir.Norm();
//
//			double radAngle = UtilityFunctions::GetRotation(nextDir, prevDir);
//			if(radAngle < _neg_curvature_param) 
//			{ 
//				_cornerPoints.push_back(curPt);		// debugging
//				bCurve->cornerFlags[b] = true;
//			}		
//		}
//	}
//	//// To do put corner flags
//}


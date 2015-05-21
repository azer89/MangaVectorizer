
/**
 *
 * Some miscellaneous functions
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#ifndef __Utility_Functions__
#define __Utility_Functions__

#define NOMINMAX
#include <iostream>
#include <vector>
#include <string>

#include "MyPoint.h"
#include "MyLine.h"
#include "MyTriangle.h"
#include "MyIndexedTriangle.h"
#include "MyIndexedBezierCurves.h"

#include "MyIndexedLine.h"

typedef std::pair<int, double> PairData;

class UtilityFunctions
{
public:

	//// Visualization
	// display image scaled (pointer array)
	static void DisplayImageDebug(double* img, int img_height, int img_width, double scale, std::string title);
	// display image scaled (opencv mat)
	static void DisplayImageDebug(cv::Mat img, double scale, std::string title);

	//// Data normalization
	static double* NormalizePhi(double* img, int img_height, int img_width);

	//// Color
	static void GetInterpolationColors(double d, int* r, int* g, int* b);
	
	//// String
	// split
	static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	// split
	static std::vector<std::string> split(const std::string &s, char delim);
	
	//// Points
	// line with thickness
	static void UtilityFunctions::GetSegmentPoints(CVSystem::MyLine curLine, CVSystem::MyLine prevLine, CVSystem::MyLine nextLine, double t0, double t1, CVSystem::MyPoint* pA, CVSystem::MyPoint* pB, CVSystem::MyPoint* pC, CVSystem::MyPoint* pD);
	// bresenham rasterization
	static std::vector<CVSystem::MyPoint> Bresenham(float x1, float y1, float x2, float y2);

	//// Angle
	// degree to radian
	static double DegreeToRadian(double deg);
	// radian to degree
	static double RadianToDegree(double rad);
	// unsigned angle
	static double AngleInBetween(CVSystem::MyPoint pt1, CVSystem::MyPoint pt2);
	// signed angle
	static double GetRotation(CVSystem::MyPoint pt1, CVSystem::MyPoint pt2);
	// angle from x axis
	static double GetAngleFromXAxis(CVSystem::MyPoint pt);
	// what quadrant ?
	static int QuadrantFromRad(double rad);
	// rotate a point around center (0, 0)
	static CVSystem::MyPoint Rotate(CVSystem::MyPoint pt, double rad);
	
	//// Maximum value
	static int GetMaxValue(int* arrayData, int numData);

	//// Sorting
	static void QuickSort(std::vector<PairData>& items, int left, int right);	
	static void QuickSortMain(std::vector<PairData>& items);

	//// Curve and resample
	static double CurveLength(std::vector<CVSystem::MyPoint> curves);
	static void UniformResample(CVSystem::MyLine oriLine, std::vector<CVSystem::MyLine>& resampleLines, int N);
	static void UniformResample(CVSystem::MyLine oriLine, std::vector<CVSystem::MyPoint>& resampleCurves, int N);
	static void UniformResample(std::vector<CVSystem::MyPoint>& oriCurve, std::vector<CVSystem::MyPoint>& resampleCurve, int N /*bool isOpen*/ ) ;

	//// Distance to line
	static double DistanceToFiniteLine(CVSystem::MyPoint v, CVSystem::MyPoint w, CVSystem::MyPoint p);

	//// Curve orientation
	static bool IsClockwise(std::vector<CVSystem::MyPoint> polygon);
	static bool IsClockwise(std::vector<int> polygon, std::vector<CVSystem::MyPoint> vertexList);

	//// Data conversion
	// point set to line set
	static std::vector<CVSystem::MyLine>		PointsToLine(std::vector<CVSystem::MyPoint> points);
	// point set to line set
	static std::vector<CVSystem::MyIndexedLine> IndexedPointsToLine(std::vector<int> points);
	// Indexed triangle mesh no non-indexed triangle mesh
	static std::vector<CVSystem::MyTriangle>	ConvertIndexedTriangles(std::vector<CVSystem::MyIndexedTriangle> triangles, std::vector<CVSystem::MyPoint> vertexList);

	//// Indexing
	// check a line exists in data
	static bool DoesExist(std::vector<CVSystem::MyIndexedLine> data, CVSystem::MyIndexedLine line);
	// shift index 
	static void ShiftIndices(std::vector<CVSystem::MyIndexedTriangle>& idxTriangles, int num);
	// shift index 
	static void ShiftIndices(std::vector<CVSystem::MyIndexedBezierCurves>& bzCurves, int num);
	// convert triangle mesh to index list
	static std::vector<int> GetDistinctIndices(std::vector<CVSystem::MyIndexedTriangle> triangles, int numVertices);
};

#endif

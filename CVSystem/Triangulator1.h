
/*
 * The most important class for line tracing and constrained triangulation
 * The input is raster image and Potrace is used to perform line tracing.
 * 
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#ifndef __Triangulator_1__
#define __Triangulator_1__

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "CSSSmoothing.h"
#include "PointInTrianglesTest.h"
#include "CGALTools.h"
#include "PointCloud.h"

extern "C" 
{
#include "potracelib.h"
#include "platform.h"
#include "bitmap_io.h"
#include "potrace_main.h"
}

#include "MyPoint.h"
#include "MyLine.h"
#include "MyTriangle.h"
#include "MyQuad.h"
#include "MyIndexedTriangle.h"
#include "MyIndexedBezierCurves.h"
#include "MyIndexedLine.h"

namespace CVSystem
{
	class Triangulator1
	{
	public:
		Triangulator1();
		~Triangulator1();
		
		void TraceImage(std::string strFilename, cv::Mat img, int* mask, int* dilatedMask);	// Trace
		
		void OCalculate();		// Original (Deprecated)
		void LSCalculate1();	// Least Square (Deprecated)
		void LSCalculate2();	// Complete Triangulation
		
		// delete these three functions
		std::vector<MyTriangle> GetOTriangles();	// Original
		std::vector<MyQuad>		GetOQuads();		// Original
		std::vector<MyTriangle> GetLSTriangles();	// LS
		
		std::vector<MyQuad> GetLSQuads();	// LS (Least Square Fitting for Cubic Bezier)

		// Indexed Format
		
		std::vector<CVSystem::MyPoint>	   GetVertexList();
		std::vector<MyIndexedTriangle>	   GetIndexedTriangles();
		std::vector<MyIndexedTriangle>	   GetWTriangles();
		std::vector<MyIndexedTriangle>	   GetSCTriangles();
		std::vector<MyIndexedTriangle>	   GetBTriangles();
		std::vector<MyIndexedTriangle>	   GetBorderWTriangles();
		std::vector<MyIndexedTriangle>	   GetBorderSCTriangles();
		std::vector<MyIndexedBezierCurves> GetIndexedBezierCurves();

		// Corner
		std::vector<CVSystem::MyPoint> GetCornerList();	 // Delete this
		std::vector<int> GetCornerIndices();
		std::vector<CVSystem::MyIndexedLine> GetCornerEdges();

		std::vector<int> GetPartOffset() { return _offsets; }

		// scaled width
		int GetWidthScaled() { return _w_scaled; }

		// scaled height
		int GetHeightScaled(){ return _h_scaled; }

	public:
		// read .myobj file
		void ReadFromMYOBJ(std::string filename);

	private:
		// stuff from Potrace
		potrace_bitmap_t* GetBM(cv::Mat img);
		void CurveToBezier(MyPoint p0, MyPoint p1, MyPoint p2, MyPoint p3, MyPoint& cp0, MyPoint& cp1);

		// create CDT		
		CVSystem::CD_Cdt GetCDT(std::vector<MyPoint> pPoly, std::vector<std::vector<MyPoint>> cPoly, bool shouldRefine = true);
		
		// create CDT
		CVSystem::CD_Cdt GetCDTWithRectangleBorder(std::vector<std::vector<MyPoint>> polygons);

		// Get hole seed
		std::list<CVSystem::CD_Point> GetHoleSeeds(std::vector<MyPoint> pPoly, std::vector<std::vector<MyPoint>> cPoly);
		
		// Assign vertex index
		void AssignVertexIndex(CVSystem::CD_Cdt& cdt);

		// Original Triangulation (Deprecated)
		std::vector<MyPoint> GetPolygonO(potrace_curve_t curve);
		std::vector<MyQuad>  GetBezierQuad0( potrace_path_t*  path);

		// Get polygon from Potrace
		std::vector<MyPoint> GetPolygonLS(potrace_path_t*  path);

		// Bezier curve fitting
		std::vector<MyQuad>  GetBezierQuadLS(std::vector<CVSystem::MyPoint> poly, std::vector<bool> rdpFlags, bool isInside);
				
		// Uniform sampling
		void Resubdivide(std::vector<CVSystem::MyPoint>& poly);

		// Clean up small polygon
		void CleanUpPoly(std::vector<CVSystem::MyPoint>& poly);

		// RDP partitioning
		void RDPSimplification(std::vector<CVSystem::MyPoint> poly, std::vector<MyPoint>& rdpPoly, std::vector<bool>& bFlag);
		
		// Build bezier curve and triangle mesh
		void ProcessEdgesFromCDT(CVSystem::CD_Cdt cd_cdt);

		// Corner Detection
		void GetCornersFromPolygon();

		// Corner Detection
		void ProcessPolygonCorners();

	private:
		std::string _strFilename;

		std::vector<MyTriangle> _oTriangles;	// deprecated
		std::vector<MyQuad>		_oQuads;		// deprecated				
		std::vector<MyTriangle> _lsTriangles;	// deprecated

		std::vector<MyQuad>		_lsQuads;		// don't delete this, it contains original LS fitting

		// New format !
		std::vector<int>				   _offsets;			 // .myobj offsets
		std::vector<MyPoint>			   _vertexList;			 // The actual vertices
		std::vector<MyIndexedTriangle>     _indexedTriangles;	 // All triangles
		std::vector<MyIndexedTriangle>     _borderSCTriangles;	 // Additional screentone triangles
		std::vector<MyIndexedTriangle>     _borderWTriangles;	 // Additional white triangles
		std::vector<MyIndexedTriangle>     _scTriangles;		 // Screentone triangles
		std::vector<MyIndexedTriangle>     _wTriangles;			 // White triangles
		std::vector<MyIndexedTriangle>     _bTriangles;			 // Black triangles
		std::vector<MyIndexedBezierCurves> _indexedBezierCurves; // Bezier curves (start and end points only)
		
		std::vector<CVSystem::MyPoint>		 _cornerList;		// Delete this
		std::vector<int>					 _cornerIndices;	// this isn't sensible
		std::vector<CVSystem::MyIndexedLine> _cornerEdges;
		
		cv::Mat _mask;						// screentone mask
		cv::Mat _dilated_mask;				// dilated screentone mask
		cv::Mat _distance_img;				// unused

		cv::Mat _mask_scaled;
		cv::Mat _dilated_mask_scaled;
		cv::Mat _distance_img_scaled;

		bool _has_tone;	// does mesh have screentone?

		int _w_scaled;	// scaled image width
		int _h_scaled;	// scaled image height

		// Gaussian refiner
		CSSSmoothing*	 _cssSmoothing;

		// Potrace
		potrace_state_t* _trace_state;
	};
}
#endif



/**
 *
 * Triangulation for context completion
 * The input is contexts.
 *
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#ifndef __Triangulator_2__
#define __Triangulator_2__

#include <vector>

// Data
#include "MyPoint.h"
#include "MyLine.h"
#include "MyTriangle.h"
#include "MyIndexedTriangle.h"
#include "MyIndexedPolygon.h"
#include "MyIndexedBezierCurves.h"

#include "IUI_StrokeType.h"

// CGAL
#include "CGALTools.h"

namespace CVSystem
{
	class Triangulator2
	{
	public:
		Triangulator2();
		~Triangulator2();

		void TriangulateAll();

		void SetData(std::vector<CVSystem::MyIndexedPolygon>      oriParts,
					 std::vector<CVSystem::MyPoint>				  oriVertexList,
					 std::vector<CVSystem::MyIndexedBezierCurves> oriIdxBzCurves,
					 std::vector<CVSystem::MyIndexedTriangle>	  oriBTriangles,
					 std::vector<CVSystem::MyIndexedTriangle>	  oriWTriangles,
					 std::vector<CVSystem::MyIndexedTriangle>	  oriSCTriangles,
					 std::vector<CVSystem::MyIndexedLine>		  oriIdxPartitionLines,
					 std::vector<IUI_StrokeType>				  oriIdxPartitionLineTypes,
					 int img_width,
					 int img_height);

	private:
		void TriangulateBaseMesh();

		void TriangulateParts();

		void TriangulateAPart(MyIndexedPolygon							    part, 
							  std::vector<CVSystem::MyPoint>			    oldVertices,
							  std::vector<CVSystem::MyPoint>&			    newVerticesTemp,
							  std::vector<CVSystem::MyIndexedTriangle>&	    newAllTTemp,
							  std::vector<CVSystem::MyIndexedTriangle>&	    newBTTemp,
							  std::vector<CVSystem::MyIndexedTriangle>&	    newWTrianglesTemp,
							  std::vector<CVSystem::MyIndexedTriangle>&	    newBorderSCTTemp,
							  std::vector<CVSystem::MyIndexedTriangle>&	    newBorderWTTemp,
							  std::vector<CVSystem::MyIndexedBezierCurves>& newIBCurvesTemp);


		CVSystem::CD_Cdt GetCDTFromLine(std::vector<CVSystem::MyPoint>					   oldVertices,
										std::vector<CVSystem::MyIndexedLine>			   pPoly, 
										std::vector<std::vector<CVSystem::MyIndexedLine>>  cPoly, 
										std::vector<CVSystem::MyPoint>					   distinctPts, 
										std::vector<int>								   distinctindices,
										bool shouldRefine);

		CVSystem::CD_Cdt GetCDTFromLineWithRectangleBorder(std::vector<CVSystem::MyPoint>					 oldVertices,
														   std::vector<CVSystem::MyIndexedLine>			     pPoly, 
														   std::vector<std::vector<CVSystem::MyIndexedLine>> cPoly, 
														   std::vector<CVSystem::MyPoint>					 distinctPts, 
														   std::vector<int>									 distinctindices,
														   bool shouldRefine);

		CVSystem::CD_Cdt GetCDTFromOriWithRectangleBorder(std::vector<CVSystem::MyPoint>	   oldVertices,
														  std::vector<int>					   distinctPLineIndices,	
														  std::vector<CVSystem::MyIndexedLine> partitionLines,
														  std::vector<std::vector<MyPoint>>	   polygons);
		
		std::list<CVSystem::CD_Point> GetHoleSeedsClosedCurve(std::vector<CVSystem::MyPoint>       oldVertices,
														      std::vector<CVSystem::MyIndexedLine> poly);

		void AssignVertexIndex(CVSystem::CD_Cdt& cdt, std::vector<CVSystem::MyPoint>& vertices);

		void ProcessEdgesFromCDT(CVSystem::CD_Cdt			    cd_cdt, 
								 std::vector<CVSystem::MyPoint> vertices,
								 std::vector<CVSystem::MyIndexedBezierCurves>& bzCurves,
								 std::vector<CVSystem::MyIndexedTriangle>& borderWTTemp,
								 std::vector<CVSystem::MyIndexedTriangle>& borderScTTemp);

	private:

		//// Original data
		std::vector<CVSystem::MyIndexedPolygon>		 _parts;

		std::vector<CVSystem::MyPoint>				 _oriVertexList;	// storing each vertex position
		std::vector<CVSystem::MyIndexedBezierCurves> _oriIdxBzCurves;	// original bezier curves

		std::vector<CVSystem::MyIndexedTriangle>	 _oriIdxBTriangles;
		std::vector<CVSystem::MyIndexedTriangle>	 _oriIdxWTriangles;
		std::vector<CVSystem::MyIndexedTriangle>	 _oriIdxSCTriangles;

		std::vector<CVSystem::MyTriangle>	 _oriBTriangles;
		std::vector<CVSystem::MyTriangle>	 _oriWTriangles;
		std::vector<CVSystem::MyTriangle>	 _oriSCTriangles;

		//// Partition Lines
		std::vector<CVSystem::MyIndexedLine> _oriIdxPartitionLines;
		std::vector<IUI_StrokeType>			 _oriIdxPartitionLineTypes;
		std::vector<int>					 _distinctPartitionLineIndices;

		//// Derived from indexedBezierCurves
		std::vector<std::vector<MyPoint>> _oriPolygons;

		// Image size
		int _img_width;
		int _img_height;

		//// New data
		std::vector<CVSystem::MyPoint>				 _nwVertexList;
		
		// All triangles
		std::vector<CVSystem::MyIndexedTriangle>	 _nwAllTriangles;
		std::vector<CVSystem::MyIndexedTriangle>	 _nwPartTriangles;
		std::vector<CVSystem::MyIndexedTriangle>	 _nwBaseTriangles;
		
		// Borders
		std::vector<CVSystem::MyIndexedTriangle>	 _nwBorderSCTriangles;
		std::vector<CVSystem::MyIndexedTriangle>	 _nwBorderWTriangles;
		
		// Screentone
		std::vector<CVSystem::MyIndexedTriangle>	 _nwSCTriangles;
		
		// White
		std::vector<CVSystem::MyIndexedTriangle>	 _nwWTriangles;
		
		// Black
		std::vector<CVSystem::MyIndexedTriangle>	 _nwBTriangles;
		
		// Curves
		std::vector<CVSystem::MyIndexedBezierCurves> _nwIndexedBezierCurves;

	};
}

#endif
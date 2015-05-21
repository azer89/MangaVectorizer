
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "Triangulator2.h"

#include "UtilityFunctions.h"
#include "PointInTrianglesTest.h"
#include "LinesSorter.h"
#include "SystemParams.h"
#include "OBJIO.h"

/**
 * 
 */
CVSystem::Triangulator2::Triangulator2()
{
}

/**
 *
 */
CVSystem::Triangulator2::~Triangulator2()
{
}

/**
 *
 */
void CVSystem::Triangulator2::TriangulateAll()
{
	TriangulateParts();

	/*
	std::vector<CVSystem::MyPoint>					newVerticesTemp;
	std::vector<CVSystem::MyIndexedTriangle>		newAllTTemp;
	std::vector<CVSystem::MyIndexedTriangle>		newBTTemp;
	std::vector<CVSystem::MyIndexedTriangle>		newWTTemp;
	std::vector<CVSystem::MyIndexedTriangle>		newSCTTemp;
	std::vector<CVSystem::MyIndexedTriangle>		newBorderSCTTemp;
	std::vector<CVSystem::MyIndexedTriangle>		newBorderWTTemp;
	std::vector<CVSystem::MyIndexedBezierCurves>	newIBCurvesTemp;

	_nwVertexList.insert(   _nwVertexList.end(),    newVerticesTemp.begin(), newVerticesTemp.end() );
	_nwAllTriangles.insert( _nwAllTriangles.end(),  newAllTTemp.begin(),     newAllTTemp.end());
	_nwPartTriangles.insert(_nwPartTriangles.end(), newAllTTemp.begin(),     newAllTTemp.end());
	//_nwBaseTriangles
	_nwBTriangles.insert(   _nwBTriangles.end(),    newBTTemp.begin(),       newBTTemp.end());
	_nwWTriangles.insert(   _nwWTriangles.end(),    newWTTemp.begin(),       newWTTemp.end());
	//_nwBorderSCTriangles
	//_nwSCTriangles
	_nwBorderWTriangles.insert(   _nwBorderWTriangles.end(),    newBorderWTTemp.begin(), newBorderWTTemp.end());
	_nwIndexedBezierCurves.insert(_nwIndexedBezierCurves.end(), newIBCurvesTemp.begin(), newIBCurvesTemp.end());

	*/
	//int part_offset = _nwAllTriangles.size();


	std::vector<int> offsets;
	offsets.resize(8);
	offsets[0] = _nwVertexList.size();			// vertex list
	offsets[1] = _nwAllTriangles.size();		// all triangles
	offsets[2] = _nwBTriangles.size();			// black
	offsets[3] = _nwWTriangles.size();			// white
	offsets[4] = 0;								// screentone
	offsets[5] = 0;								// border screentone
	offsets[6] = _nwBorderWTriangles.size();	// border white
	offsets[7] = _nwIndexedBezierCurves.size();	// bezier curve

	TriangulateBaseMesh();

	//std::cout << "Triangulation2: " << SystemParams::str_filename << "\n";
	std::vector<std::string> fullpath = UtilityFunctions::split(SystemParams::str_filename, '//');
	std::vector<std::string> nameArray = UtilityFunctions::split(fullpath[fullpath.size() - 1], '.');
	std::string meshFilename = "Mesh_Parts/" + nameArray[0] + ".myobj";

	// We don't have lsQuads
	std::vector<MyQuad> resizedLsQuads;
	OBJIO::WriteMyToOBJ(meshFilename, 
						_img_width,
						_img_height,
						offsets,
						_nwVertexList, 
						_nwIndexedBezierCurves,
						_nwBorderSCTriangles,
						_nwBorderWTriangles,
						resizedLsQuads,		// resized
						_nwAllTriangles);
}

/**
 *
 */
void CVSystem::Triangulator2::TriangulateParts()
{
	int num = 0;

	for(size_t a = 0; a < _parts.size(); a++)
	{
		std::vector<CVSystem::MyPoint>				 newVerticesTemp;
		std::vector<CVSystem::MyIndexedTriangle>	 newAllTTemp;
		std::vector<CVSystem::MyIndexedTriangle>	 newBTTemp;
		std::vector<CVSystem::MyIndexedTriangle>	 newWTTemp;
		std::vector<CVSystem::MyIndexedTriangle>	 newBorderSCTTemp;
		std::vector<CVSystem::MyIndexedTriangle>	 newBorderWTTemp;
		std::vector<CVSystem::MyIndexedBezierCurves> newIBCurvesTemp;

		TriangulateAPart(this->_parts[a], 
						 this->_oriVertexList,
						 newVerticesTemp,
						 newAllTTemp,
						 newBTTemp,
						 newWTTemp,
						 newBorderSCTTemp,
						 newBorderWTTemp,
						 newIBCurvesTemp);

		if(num > 0)
		{
			UtilityFunctions::ShiftIndices(newAllTTemp,       num);
			UtilityFunctions::ShiftIndices(newBTTemp,         num);
			UtilityFunctions::ShiftIndices(newWTTemp,		  num);
			UtilityFunctions::ShiftIndices(newBorderSCTTemp,  num);
			UtilityFunctions::ShiftIndices(newBorderWTTemp,   num);
			UtilityFunctions::ShiftIndices(newIBCurvesTemp,   num);
		}

		num += newVerticesTemp.size();

		_nwVertexList.insert(   _nwVertexList.end(),    newVerticesTemp.begin(), newVerticesTemp.end() );
		_nwAllTriangles.insert( _nwAllTriangles.end(),  newAllTTemp.begin(),     newAllTTemp.end());
		_nwPartTriangles.insert(_nwPartTriangles.end(), newAllTTemp.begin(),     newAllTTemp.end());
		//_nwBaseTriangles
		_nwBTriangles.insert(   _nwBTriangles.end(),    newBTTemp.begin(),       newBTTemp.end());
		_nwWTriangles.insert(   _nwWTriangles.end(),    newWTTemp.begin(),       newWTTemp.end());
		//_nwBorderSCTriangles
		//_nwSCTriangles
		_nwBorderWTriangles.insert(   _nwBorderWTriangles.end(),    newBorderWTTemp.begin(), newBorderWTTemp.end());
		_nwIndexedBezierCurves.insert(_nwIndexedBezierCurves.end(), newIBCurvesTemp.begin(), newIBCurvesTemp.end());
	}
}

/**
 *
 */
void CVSystem::Triangulator2::TriangulateBaseMesh()
{
	std::vector<CVSystem::MyPoint>				 newVerticesTemp;
	std::vector<CVSystem::MyIndexedTriangle>	 newAllTTemp;
	std::vector<CVSystem::MyIndexedTriangle>	 newBTTemp;
	std::vector<CVSystem::MyIndexedTriangle>	 newWTTemp;
	std::vector<CVSystem::MyIndexedTriangle>	 newSCTTemp;
	std::vector<CVSystem::MyIndexedTriangle>	 newBorderSCTTemp;
	std::vector<CVSystem::MyIndexedTriangle>	 newBorderWTTemp;
	std::vector<CVSystem::MyIndexedBezierCurves> newIBCurvesTemp;

	CD_Cdt cd_cdt = GetCDTFromOriWithRectangleBorder(_oriVertexList,
													 _distinctPartitionLineIndices,	
													 _oriIdxPartitionLines,
													 _oriPolygons);

	// A seed on the border
	std::list<CD_Point> seeds;
	seeds.push_back(CD_Point(1, 1));
	CGAL::refine_Delaunay_mesh_2(cd_cdt, 
								seeds.begin(), 
								seeds.end(), 
								CD_Criteria(SystemParams::t_delaunay_aspect_bound, 
								SystemParams::t_delaunay_max_length));

	AssignVertexIndex(cd_cdt, newVerticesTemp);

	PointInTrianglesTest* bTest = new PointInTrianglesTest();
	bTest->PushTriangles(_oriBTriangles);

	std::vector<CVSystem::MyTriangle> prTriangles = UtilityFunctions::ConvertIndexedTriangles(_nwPartTriangles, _nwVertexList);
	PointInTrianglesTest* partTest = new PointInTrianglesTest();
	partTest->PushTriangles(prTriangles);

	if(_oriIdxSCTriangles.size() > 0)
	{
		//// Has screentone

		PointInTrianglesTest* scTest = new PointInTrianglesTest();
		scTest->PushTriangles(_oriSCTriangles);

		for(CD_Cdt::Finite_faces_iterator fit = cd_cdt.finite_faces_begin(); fit != cd_cdt.finite_faces_end(); ++fit) 
		{
			if(fit->is_in_domain())
			{
				TriangleType triType = TRIANGLE_NOT_DEFINED;
				if(bTest->IsInside(fit))
				{
					if(partTest->IsInside(fit)) { triType = TRIANGLE_WHITE; }
					else { triType = TRIANGLE_BLACK; }
					//triType = TRIANGLE_BLACK;
				}
				else if(scTest->IsInside(fit)) 
				{ 
					triType = TRIANGLE_SCREENTONE; 
				}
				else { triType = TRIANGLE_WHITE; }

				fit->triangleType = triType;
				MyIndexedTriangle tri( fit->vertex(0)->info(), fit->vertex(1)->info(), fit->vertex(2)->info(), triType);
				newAllTTemp.push_back(tri);
				if     (triType == TRIANGLE_SCREENTONE) { newSCTTemp.push_back(tri); }
				else if(triType == TRIANGLE_WHITE)		{ newWTTemp.push_back(tri); }
				else if(triType == TRIANGLE_BLACK)		{ newBTTemp.push_back(tri); }
			}
		}
	}
	else
	{
		//// Doesn't have screentone
		for(CD_Cdt::Finite_faces_iterator fit = cd_cdt.finite_faces_begin(); fit != cd_cdt.finite_faces_end(); ++fit) 
		{
			if(fit->is_in_domain())
			{
				TriangleType triType = TRIANGLE_NOT_DEFINED;
				if(bTest->IsInside(fit))
				{
					if(partTest->IsInside(fit)) { triType = TRIANGLE_WHITE; }
					else { triType = TRIANGLE_BLACK; }
					//triType = TRIANGLE_BLACK;
				}
				else
					{ triType = TRIANGLE_WHITE; }

				fit->triangleType = triType;
				MyIndexedTriangle tri( fit->vertex(0)->info(), fit->vertex(1)->info(), fit->vertex(2)->info(), triType);
				newAllTTemp.push_back(tri);
				if(triType == TRIANGLE_WHITE)		{ newWTTemp.push_back(tri); }
				else if(triType == TRIANGLE_BLACK)	{ newBTTemp.push_back(tri); }
			}
		}
	}

	ProcessEdgesFromCDT(cd_cdt,
						newVerticesTemp,
						newIBCurvesTemp,
						newBorderWTTemp,
						newBorderSCTTemp);

	int num = _nwVertexList.size();
	if(num > 0)
	{
		UtilityFunctions::ShiftIndices(newAllTTemp,       num);
		UtilityFunctions::ShiftIndices(newBTTemp,         num);
		UtilityFunctions::ShiftIndices(newWTTemp,		  num);
		UtilityFunctions::ShiftIndices(newSCTTemp,		  num);
		UtilityFunctions::ShiftIndices(newBorderSCTTemp,  num);
		UtilityFunctions::ShiftIndices(newBorderWTTemp,   num);
		UtilityFunctions::ShiftIndices(newIBCurvesTemp,   num);
	}

	_nwVertexList.insert( _nwVertexList.end(), newVerticesTemp.begin(), newVerticesTemp.end() );
	_nwAllTriangles.insert(_nwAllTriangles.end(), newAllTTemp.begin(), newAllTTemp.end());
	//_nwPartTriangles
	_nwBaseTriangles.insert(_nwBaseTriangles.end(), newAllTTemp.begin(), newAllTTemp.end());
	_nwBTriangles.insert(_nwBTriangles.end(), newBTTemp.begin(), newBTTemp.end());
	_nwWTriangles.insert(_nwWTriangles.end(), newWTTemp.begin(), newWTTemp.end());

	_nwBorderSCTriangles.insert(_nwBorderSCTriangles.end(), newBorderSCTTemp.begin(), newBorderSCTTemp.end());
	_nwSCTriangles.insert(_nwSCTriangles.end(), newSCTTemp.begin(), newSCTTemp.end());

	_nwBorderWTriangles.insert(   _nwBorderWTriangles.end(),    newBorderWTTemp.begin(), newBorderWTTemp.end());
	_nwIndexedBezierCurves.insert(_nwIndexedBezierCurves.end(), newIBCurvesTemp.begin(), newIBCurvesTemp.end());
}

/**
 *
 */
void CVSystem::Triangulator2::TriangulateAPart(MyIndexedPolygon							        part, 
											   std::vector<CVSystem::MyPoint>			        oldVertices,
											   std::vector<CVSystem::MyPoint>&			        newVerticesTemp,
											   std::vector<CVSystem::MyIndexedTriangle>&	    newAllTTemp,
											   std::vector<CVSystem::MyIndexedTriangle>&	    newBTTemp,
											   std::vector<CVSystem::MyIndexedTriangle>&	    newWTrianglesTemp,
											   std::vector<CVSystem::MyIndexedTriangle>&	    newBorderSCTTemp,
											   std::vector<CVSystem::MyIndexedTriangle>&	    newBorderWTTemp,
											   std::vector<CVSystem::MyIndexedBezierCurves>&    newIBCurvesTemp)
{
	std::vector<MyTriangle> bTTemp;
	CD_Cdt cdt_blk = GetCDTFromLine(oldVertices, part.boundary, part.holes, part.distinctPoints, part.distinctIndices, false);

	for(CD_Cdt::Finite_faces_iterator fit = cdt_blk.finite_faces_begin(); fit != cdt_blk.finite_faces_end(); ++fit) 
	{
		if(fit->is_in_domain()) 
		{
			MyTriangle tr = MyTriangle( MyPoint(fit->vertex(0)->point().x(), fit->vertex(0)->point().y()), 
										MyPoint(fit->vertex(1)->point().x(), fit->vertex(1)->point().y()), 
										MyPoint(fit->vertex(2)->point().x(), fit->vertex(2)->point().y()), 
										TRIANGLE_BLACK);
			bTTemp.push_back(tr);
		}
	}		

	CD_Cdt cdt_final = GetCDTFromLineWithRectangleBorder(oldVertices, part.boundary, part.holes, part.distinctPoints, part.distinctIndices, false);

	std::list<CD_Point> seeds;
	seeds.push_back(CD_Point(1, 1));

	CGAL::refine_Delaunay_mesh_2(cdt_final, seeds.begin(), seeds.end(), CD_Criteria(SystemParams::t_delaunay_aspect_bound, SystemParams::t_delaunay_max_length));
		
	AssignVertexIndex(cdt_final, newVerticesTemp);

	PointInTrianglesTest* bTest = new PointInTrianglesTest();
	bTest->PushTriangles(bTTemp);
	

	for(CD_Cdt::Finite_faces_iterator fit = cdt_final.finite_faces_begin(); fit != cdt_final.finite_faces_end(); ++fit) 
	{
		if(fit->is_in_domain())
		{
			TriangleType triType = (bTest->IsInside(fit)) ? TRIANGLE_BLACK : TRIANGLE_WHITE;
			MyIndexedTriangle tri(fit->vertex(0)->info(), fit->vertex(1)->info(), fit->vertex(2)->info(), triType);
			newAllTTemp.push_back(tri);
			fit->triangleType = triType;

			if(triType == TRIANGLE_WHITE) { newWTrianglesTemp.push_back(tri); }
			else if(triType == TRIANGLE_BLACK) { newBTTemp.push_back(tri); }
		}
	}

	ProcessEdgesFromCDT(cdt_final,
						_oriVertexList,
						newIBCurvesTemp,
						newBorderWTTemp,
						newBorderSCTTemp);
}

/**
 *
 */
CVSystem::CD_Cdt 
	CVSystem::Triangulator2::GetCDTFromLine(std::vector<CVSystem::MyPoint>					   oldVertices,
											std::vector<CVSystem::MyIndexedLine>			   pPoly, 
											std::vector<std::vector<CVSystem::MyIndexedLine>>  cPoly, 
											std::vector<CVSystem::MyPoint>					   distinctPts, 
											std::vector<int>								   distinctindices,
											bool shouldRefine)
{
	// PRACTICAL SOLUTION
	/*std::vector<bool> stupidFlags;
	stupidFlags.resize(oldVertices.size());
	for(size_t a = 0; a < stupidFlags.size(); a++)
		{ stupidFlags[a] = false; }*/

	CD_Cdt cd_cdt;
	std::vector<CD_VHandle> vHandles;
	vHandles.resize(oldVertices.size());

	for(size_t a = 0; a < distinctPts.size(); a++)		
	{ 
		CD_VHandle vH = cd_cdt.insert(CD_Point(distinctPts[a].x, distinctPts[a].y));
		vHandles[distinctindices[a]] = vH;
	}

	//// Get hole seeds
	std::list<CD_Point> seeds;	
	for(size_t a = 0; a < cPoly.size(); a++)
	{
		std::list<CD_Point> tempSeeds = GetHoleSeedsClosedCurve(oldVertices, cPoly[a]);
		seeds.insert( seeds.end(), tempSeeds.begin(), tempSeeds.end() );
	}


	//// DEBUG
	//cv::Mat img(_img_height, _img_height, CV_8UC1);
	//img = 255;
	//// DEBUG
	//for(std::list<CD_Point>::iterator it = seeds.begin(); it != seeds.end(); it++)////
	//{
	//	int x1 = it->x();////
	//	int y1 = it->y();////
	//	img.ptr<uchar>(y1, x1)[0] = 0;////
	//}////

	//// set constraints
	for(size_t a = 0; a < pPoly.size(); a++)
	{ 
		MyPoint pt1 = oldVertices[pPoly[a].index0];
		MyPoint pt2 = oldVertices[pPoly[a].index1];

		double dist = pt1.Distance(pt2);
		//std::cout << dist << "\n";

		if(dist > M_EPS /*&& (!stupidFlags[pPoly[a].index0] || !stupidFlags[pPoly[a].index1])*/ )
		{
			cd_cdt.insert_constraint(vHandles[pPoly[a].index0], vHandles[pPoly[a].index1]); 

			//stupidFlags[pPoly[a].index0] = true; 
			//stupidFlags[pPoly[a].index1] = true; 
		}
	}

	for(size_t a = 0; a < cPoly.size(); a++)
	{
		std::vector<CVSystem::MyIndexedLine> poly = cPoly[a];
		for(size_t b = 0; b < poly.size(); b++)
		{ 
			MyPoint pt1 = oldVertices[poly[b].index0];
			MyPoint pt2 = oldVertices[poly[b].index1];

			double dist = pt1.Distance(pt2);
			//std::cout << dist << "\n";

			if(dist > M_EPS /*&& (!stupidFlags[poly[a].index0] || !stupidFlags[poly[a].index1])*/)
			{
				//std::cout << "(" << pt1.x << ", " << pt1.y << ") (" << pt2.x << ", " << pt2.y << ")\n";

				cd_cdt.insert_constraint(vHandles[poly[b].index0], vHandles[poly[b].index1]);

				//stupidFlags[poly[a].index0] = true; 
				//stupidFlags[poly[a].index1] = true;
			}
		}
	}
	if(shouldRefine) { CGAL::refine_Delaunay_mesh_2(cd_cdt, seeds.begin(), seeds.end(), CD_Criteria(SystemParams::t_delaunay_aspect_bound, SystemParams::t_delaunay_max_length)); }
	else
	{ CGAL::refine_Delaunay_mesh_2(cd_cdt, seeds.begin(), seeds.end(), CD_Criteria(0.01, SystemParams::t_delaunay_max_length)); }

	////// DEBUG
	//for(CD_Cdt::Finite_faces_iterator fit = cd_cdt.finite_faces_begin(); fit != cd_cdt.finite_faces_end(); ++fit) ////
	//{
	//	if(fit->is_in_domain())////
	//	{
	//		// DEBUG
	//		int x1 = (*(fit->vertex(0))).point().x();////
	//		int y1 = (*(fit->vertex(0))).point().y();////
	//		int x2 = (*(fit->vertex(1))).point().x();////
	//		int y2 = (*(fit->vertex(1))).point().y();////
	//		int x3 = (*(fit->vertex(2))).point().x();////
	//		int y3 = (*(fit->vertex(2))).point().y();////
	//		// DEBUG
	//		cv::line(img, cv::Point(x1, y1), cv::Point(x2, y2), 127, 1);////
	//		cv::line(img, cv::Point(x2, y2), cv::Point(x3, y3), 127, 1);////
	//		cv::line(img, cv::Point(x3, y3), cv::Point(x1, y1), 127, 1);////
	//	}
	//}
	//cv::imwrite("domdom.png", img);////

	return cd_cdt;
}

/**
 *
 */
CVSystem::CD_Cdt 
	CVSystem::Triangulator2::GetCDTFromOriWithRectangleBorder(std::vector<CVSystem::MyPoint>	          oldVertices,
															  std::vector<int>					          distinctPLineIndices,
															  std::vector<CVSystem::MyIndexedLine>        partitionLines,
															  std::vector<std::vector<CVSystem::MyPoint>> polygons)
{
	using namespace CVSystem;

	CD_Cdt cd_cdt;
	std::vector<std::vector<CD_VHandle>> vHandles;

	std::vector<MyPoint> border;
	border.push_back(MyPoint(0,					0));
	border.push_back(MyPoint(this->_img_width,	0));
	border.push_back(MyPoint(this->_img_width,	this->_img_height));
	border.push_back(MyPoint(0,					this->_img_height));

	for(size_t a = 0; a < polygons.size(); a++)
	{
		std::vector<CVSystem::MyPoint> cPs = polygons[a];
		std::vector<CD_VHandle> hdls;
		for(size_t b = 0; b < cPs.size() - 1; b++)  { hdls.push_back( cd_cdt.insert(CD_Point(cPs[b].x, cPs[b].y)) );  }
		vHandles.push_back(hdls);
	}

	std::vector<CD_VHandle> borderVHandles;
	for(size_t a = 0; a < border.size(); a++) { borderVHandles.push_back( cd_cdt.insert(CD_Point(border[a].x, border[a].y)) );  }
	vHandles.push_back(borderVHandles);
	for(size_t a = 0; a < vHandles.size(); a++)
	{
		std::vector<CD_VHandle> hdls = vHandles[a];
		for(size_t b = 0; b < hdls.size(); b++)
		{
			if(b == 0) 
			{ 
				if(hdls[hdls.size() - 1] != hdls[b])
				{
					cd_cdt.insert_constraint(hdls[hdls.size() - 1], hdls[b]); 
				}
				
			}
			else 
			{ 
				if(hdls[b-1] != hdls[b])
				{
					cd_cdt.insert_constraint(hdls[b-1], hdls[b]); 
				}
				
			}
		}
	}

	//// Partition Line
	std::vector<CD_VHandle> vHandleTemp;
	vHandleTemp.resize(oldVertices.size());
	for(size_t a = 0; a < distinctPLineIndices.size(); a++)		//// Partition Line
	{ 
		MyPoint pt = oldVertices[distinctPLineIndices[a]];
		CD_VHandle vH = cd_cdt.insert(CD_Point(pt.x, pt.y));
		vHandleTemp[distinctPLineIndices[a]] = vH;
	}
	for(size_t a = 0; a < partitionLines.size(); a++)			//// Partition Line
	{

		MyPoint pt1 = oldVertices[partitionLines[a].index0];
		MyPoint pt2 = oldVertices[partitionLines[a].index1];

		double dist = pt1.Distance(pt2);
		if(dist > M_EPS)
		{

		CD_VHandle vhA = vHandleTemp[partitionLines[a].index0];
		CD_VHandle vhB = vHandleTemp[partitionLines[a].index1];
		cd_cdt.insert_constraint(vhA, vhB);
		}
	}

	CGAL::refine_Delaunay_mesh_2(cd_cdt, CD_Criteria(SystemParams::t_delaunay_aspect_bound, SystemParams::t_delaunay_max_length));
	return cd_cdt;
}

/**
 *
 */
CVSystem::CD_Cdt 
	CVSystem::Triangulator2::GetCDTFromLineWithRectangleBorder(std::vector<CVSystem::MyPoint>					 oldVertices,
															   std::vector<CVSystem::MyIndexedLine>			     pPoly, 
															   std::vector<std::vector<CVSystem::MyIndexedLine>> cPoly, 
															   std::vector<CVSystem::MyPoint>					 distinctPts, 
															   std::vector<int>									 distinctindices,
															   bool shouldRefine)
{
	// PRACTICAL SOLUTION
	/*std::vector<bool> stupidFlags;
	stupidFlags.resize(oldVertices.size());
	for(size_t a = 0; a < stupidFlags.size(); a++)
	{ stupidFlags[a] = false; }*/

	CD_Cdt cd_cdt;

	//// This vector is inefficient
	std::vector<CD_VHandle> vHandles;
	vHandles.resize(oldVertices.size());

	for(size_t a = 0; a < distinctPts.size(); a++)		
	{ 
		CD_VHandle vH = cd_cdt.insert(CD_Point(distinctPts[a].x, distinctPts[a].y));
		vHandles[distinctindices[a]] = vH;
	}
	
	//// set constraints
	for(size_t a = 0; a < pPoly.size(); a++)
	{
		MyPoint pt1 = oldVertices[pPoly[a].index0];
		MyPoint pt2 = oldVertices[pPoly[a].index1];

		double dist = pt1.Distance(pt2);

		if(dist > M_EPS /*&& (!stupidFlags[pPoly[a].index0] || !stupidFlags[pPoly[a].index1]) */ )
		{

			CD_VHandle vhA = vHandles[pPoly[a].index0];
			CD_VHandle vhB = vHandles[pPoly[a].index1];

			//stupidFlags[pPoly[a].index0] = true; 
			//stupidFlags[pPoly[a].index1] = true; 


			cd_cdt.insert_constraint(vhA, vhB);
		}
	}

	for(size_t a = 0; a < cPoly.size(); a++)
	{
		std::vector<CVSystem::MyIndexedLine> poly = cPoly[a];
		for(size_t b = 0; b < poly.size(); b++)
		{

			MyPoint pt1 = oldVertices[poly[b].index0];
			MyPoint pt2 = oldVertices[poly[b].index1];

			double dist = pt1.Distance(pt2);

			if(dist > M_EPS  /* && (!stupidFlags[poly[b].index0] || !stupidFlags[poly[b].index1]) */ )
			{

				CD_VHandle vhA = vHandles[poly[b].index0];
				CD_VHandle vhB = vHandles[poly[b].index1];

				//stupidFlags[poly[b].index0] = true; 
				//stupidFlags[poly[b].index1] = true; 

				cd_cdt.insert_constraint(vhA, vhB);

			}
		}
	}

	std::vector<MyPoint> border;
	border.push_back(MyPoint(0,					0));
	border.push_back(MyPoint(this->_img_width,	0));
	border.push_back(MyPoint(this->_img_width,	this->_img_height));
	border.push_back(MyPoint(0,					this->_img_height));
	std::vector<CD_VHandle> borderVHandles;
	for(size_t a = 0; a < border.size(); a++) 
		{ borderVHandles.push_back( cd_cdt.insert(CD_Point(border[a].x, border[a].y)) );  }

	for(size_t b = 0; b < borderVHandles.size(); b++)
	{
		if(b == 0) { cd_cdt.insert_constraint(borderVHandles[borderVHandles.size() - 1], borderVHandles[b]); }
		else { cd_cdt.insert_constraint(borderVHandles[b-1], borderVHandles[b]); }
	}

	CGAL::refine_Delaunay_mesh_2(cd_cdt, CD_Criteria(SystemParams::t_delaunay_aspect_bound, SystemParams::t_delaunay_max_length));
	
	return cd_cdt;
}

/**
 * Only closed curve, this function does not support curves with intersection
 */
std::list<CVSystem::CD_Point> 
	CVSystem::Triangulator2::GetHoleSeedsClosedCurve(std::vector<CVSystem::MyPoint> oldVertices,
												     std::vector<CVSystem::MyIndexedLine> poly)
{
	using namespace CVSystem;

	std::list<CD_Point> seeds;

	CD_Cdt cd_cdt;
	std::vector<CD_VHandle> vHandles;

	for(size_t a = 0; a < poly.size(); a++)
	{
		MyPoint pt = oldVertices[poly[a].index1];
		vHandles.push_back(  cd_cdt.insert(CD_Point(pt.x, pt.y))); 
	}

	cv::Mat img(this->_img_height, this->_img_height, CV_8UC1);	////
	img = 255; ////

	for(size_t a = 1; a < vHandles.size(); a++)
	{ 
		double x1 = vHandles[a - 1]->point().x();	////
		double y1 = vHandles[a - 1]->point().y();////
		double x2 = vHandles[a ]->point().x();////
		double y2 = vHandles[a ]->point().y();////

		double dist = MyPoint(x1, y1).Distance(MyPoint(x2, y2));
		//std::cout << dist << "\n";

		if(dist >-M_EPS && dist < M_EPS) { continue; }

		cd_cdt.insert_constraint(vHandles[a - 1], vHandles[a]); 

		

		//cv::line(img, cv::Point(x1, y1), cv::Point(x2, y2), 0, 1);////

	}
	cd_cdt.insert_constraint(vHandles[vHandles.size() - 1], vHandles[0]); 

	//int x1 = vHandles[vHandles.size() - 1]->point().x();////
	//int y1 = vHandles[vHandles.size() - 1]->point().y();////
	//int x2 = vHandles[0 ]->point().x();////
	//int y2 = vHandles[0 ]->point().y();////
	//cv::line(img, cv::Point(x1, y1), cv::Point(x2, y2), 0, 1);////

	std::vector<MyPoint> border;
	border.push_back(MyPoint(0,					0));
	border.push_back(MyPoint(this->_img_width,	0));
	border.push_back(MyPoint(this->_img_width,	this->_img_height));
	border.push_back(MyPoint(0,					this->_img_height));
	std::vector<CD_VHandle> borderVHandles;
	for(size_t a = 0; a < border.size(); a++) 
	{ borderVHandles.push_back( cd_cdt.insert(CD_Point(border[a].x, border[a].y)) );  }
	std::list<CD_Point> holeSeeds;
	holeSeeds.push_back(CD_Point(1, 1));
	CGAL::refine_Delaunay_mesh_2(cd_cdt, holeSeeds.begin(), holeSeeds.end(), CD_Criteria(SystemParams::t_delaunay_aspect_bound, SystemParams::t_delaunay_max_length));

	for (CD_Cdt::Finite_faces_iterator fit = cd_cdt.finite_faces_begin(); fit != cd_cdt.finite_faces_end(); ++fit)
	{	
		if ( fit->is_in_domain())
		{
			float x_center = ((*(fit->vertex(0))).point().x() + (*(fit->vertex(1))).point().x() + (*(fit->vertex(2))).point().x()) / 3.0;
			float y_center = ((*(fit->vertex(0))).point().y() + (*(fit->vertex(1))).point().y() + (*(fit->vertex(2))).point().y()) / 3.0;
			seeds.push_back(CD_Point(x_center, y_center));


			//img.ptr<uchar>(y_center, x_center)[0] = 0;////
			//int x1 = (*(fit->vertex(0))).point().x();////
			//int y1 = (*(fit->vertex(0))).point().y();////
			//int x2 = (*(fit->vertex(1))).point().x();////
			//int y2 = (*(fit->vertex(1))).point().y();////
			//int x3 = (*(fit->vertex(2))).point().x();////
			//int y3 = (*(fit->vertex(2))).point().y();////
			//cv::line(img, cv::Point(x1, y1), cv::Point(x2, y2), 127, 1);////
			//cv::line(img, cv::Point(x2, y2), cv::Point(x3, y3), 127, 1);////
			//cv::line(img, cv::Point(x3, y3), cv::Point(x1, y1), 127, 1);////

		}
	}
	//cv::imwrite("dumdum.png", img);////

	return seeds;
}


/**
 * Add index on vertices similar to .OBJ file format
 */ 
void CVSystem::Triangulator2::AssignVertexIndex(CVSystem::CD_Cdt& cdt, std::vector<CVSystem::MyPoint>& newVertices)
{
	//// Make all invalid
	for (CD_Cdt::Finite_vertices_iterator vit = cdt.finite_vertices_begin(); vit != cdt.finite_vertices_end(); ++vit) 
	{ vit->info() = -2; }

	for(CD_Cdt::Finite_faces_iterator fit = cdt.finite_faces_begin(); fit != cdt.finite_faces_end(); ++fit) 
	{ if(fit->is_in_domain()) { fit->vertex(0)->info() = -1; fit->vertex(1)->info() = -1; fit->vertex(2)->info() = -1; } }

	//// Start assigning index
	int idxCounter = 0;
	for (CD_Cdt::Finite_vertices_iterator vit = cdt.finite_vertices_begin(); vit != cdt.finite_vertices_end(); ++vit) 
	{ if(vit->info() == -1) { vit->info() = idxCounter++; } }

	//// Add indexed vertices to list
	for (CD_Cdt::Finite_vertices_iterator vit = cdt.finite_vertices_begin(); vit != cdt.finite_vertices_end(); ++vit) 
	{ if(vit->info() >= 0) { newVertices.push_back(MyPoint(vit->point().x(), vit->point().y())); } }
}

/**
 *
 */
void  
	CVSystem::Triangulator2::ProcessEdgesFromCDT(CVSystem::CD_Cdt								cd_cdt, 
												 std::vector<CVSystem::MyPoint>					oldVertices,
												 std::vector<CVSystem::MyIndexedBezierCurves>&	bzCurves,
												 std::vector<CVSystem::MyIndexedTriangle>&		borderWTTemp,
												 std::vector<CVSystem::MyIndexedTriangle>&		borderScTTemp)
{
	std::vector<MyIndexedLine> unsortedLines;

	for (CD_Cdt::Finite_edges_iterator eit = cd_cdt.finite_edges_begin(); eit != cd_cdt.finite_edges_end(); ++eit) 
	{
		const CD_Cdt::Face_handle& fh1 = eit->first;					// left?
		const CD_Cdt::Face_handle& fh2 = fh1->neighbor(eit->second);	// right?

		if(fh1->triangleType == TRIANGLE_BLACK && fh2->triangleType != TRIANGLE_BLACK)
		{
			int i = eit->second;			
			int index0 = fh1->vertex(fh1->cw(i))->info();
			int index1 = fh1->vertex(fh1->ccw(i))->info();
			unsortedLines.push_back(MyIndexedLine(index0, index1));
		}

		//// should be flipped so black triangles are in left side
		if(fh1->triangleType != TRIANGLE_BLACK && fh2->triangleType == TRIANGLE_BLACK)
		{
			int i = eit->second;					
			int index0 = fh1->vertex(fh1->cw(i))->info();
			int index1 = fh1->vertex(fh1->ccw(i))->info();
			unsortedLines.push_back(MyIndexedLine(index1, index0));
		}

		if(fh1->triangleType == TRIANGLE_BLACK && fh2->triangleType == TRIANGLE_WHITE)		   
			{ borderWTTemp.push_back( MyIndexedTriangle( fh1->vertex(0)->info(), fh1->vertex(1)->info(), fh1->vertex(2)->info(), TRIANGLE_WHITE)); }
		else if(fh1->triangleType == TRIANGLE_WHITE && fh2->triangleType == TRIANGLE_BLACK)
			{ borderWTTemp.push_back( MyIndexedTriangle( fh2->vertex(0)->info(), fh2->vertex(1)->info(), fh2->vertex(2)->info(), TRIANGLE_WHITE)); }
		else if(fh1->triangleType == TRIANGLE_BLACK && fh2->triangleType == TRIANGLE_SCREENTONE)
			{ borderScTTemp.push_back( MyIndexedTriangle( fh1->vertex(0)->info(), fh1->vertex(1)->info(), fh1->vertex(2)->info(), TRIANGLE_SCREENTONE)); }
		else if(fh1->triangleType == TRIANGLE_SCREENTONE && fh2->triangleType == TRIANGLE_BLACK)
			{ borderScTTemp.push_back( MyIndexedTriangle( fh2->vertex(0)->info(), fh2->vertex(1)->info(),  fh2->vertex(2)->info(), TRIANGLE_SCREENTONE)); }

	}

	// bug ?
	//if(unsortedLines.size() > 0)
	//{
		bzCurves = LinesSorter::BuildBezierCurves2(unsortedLines, oldVertices);
	//}
}

/**
 *
 */
void CVSystem::Triangulator2::SetData(std::vector<CVSystem::MyIndexedPolygon>      oriParts,
									  std::vector<CVSystem::MyPoint>			   oriVertexList,
									  std::vector<CVSystem::MyIndexedBezierCurves> oriIdxBzCurves,
									  std::vector<CVSystem::MyIndexedTriangle>	   oriBTriangles,
									  std::vector<CVSystem::MyIndexedTriangle>	   oriWTriangles,
									  std::vector<CVSystem::MyIndexedTriangle>	   oriSCTriangles,
									  std::vector<CVSystem::MyIndexedLine>		   oriIdxPartitionLines,
									  std::vector<IUI_StrokeType>				   oriIdxPartitionLineTypes,
									  int img_width,
									  int img_height)
{
	this->_parts		     = oriParts;
	this->_oriVertexList     = oriVertexList;
	this->_oriIdxBzCurves    = oriIdxBzCurves;
	this->_oriIdxBTriangles  = oriBTriangles;
	this->_oriIdxWTriangles  = oriWTriangles;
	this->_oriIdxSCTriangles = oriSCTriangles;

	this->_oriIdxPartitionLines		= oriIdxPartitionLines;
	this->_oriIdxPartitionLineTypes = oriIdxPartitionLineTypes;

	//// convert to poly
	for(size_t a = 0; a < oriIdxBzCurves.size(); a++)
	{
		MyIndexedBezierCurves curve = oriIdxBzCurves[a];
		std::vector<CVSystem::MyPoint> poly;
		for(size_t b = 0; b < curve.indices.size() - 1; b++)
		{ poly.push_back(_oriVertexList[curve.indices[b]]); }
		_oriPolygons.push_back(poly);
	}

	//// Image size
	this->_img_width = img_width;
	this->_img_height = img_height;

	//// Distinct partition lines indices
	_distinctPartitionLineIndices.clear();
	std::vector<bool> flags;
	for(size_t a = 0; a < _oriVertexList.size(); a++)
		{ flags.push_back(false); }
	for(size_t a = 0; a < _oriIdxPartitionLines.size(); a++)
	{
		int idx0 = _oriIdxPartitionLines[a].index0;
		int idx1 = _oriIdxPartitionLines[a].index1;

		if(!flags[idx0])
		{
			_distinctPartitionLineIndices.push_back(idx0);
			flags[idx0] = true;
		}

		if(!flags[idx1])
		{
			_distinctPartitionLineIndices.push_back(idx1);
			flags[idx1] = true;
		}	
	}

	//// Non-indexed
	_oriBTriangles  = UtilityFunctions::ConvertIndexedTriangles(_oriIdxBTriangles,  _oriVertexList);
	_oriWTriangles  = UtilityFunctions::ConvertIndexedTriangles(_oriIdxWTriangles,  _oriVertexList);
	_oriSCTriangles = UtilityFunctions::ConvertIndexedTriangles(_oriIdxSCTriangles, _oriVertexList);
}

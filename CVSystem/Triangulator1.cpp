
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "Triangulator1.h"

#include "CurveRDP.h"
#include "CurveInterpolation.h"
#include "OBJIO.h"
#include "SystemParams.h"
#include "CurveFitting.h"
#include "PixelsInTriangle.h"
#include "LinesSorter.h"
#include "UtilityFunctions.h"

#include <boost/chrono.hpp>

CVSystem::Triangulator1::Triangulator1() : 
	_w_scaled(0),
	_h_scaled(0),
	_trace_state(0),
	_has_tone(false),
	_cssSmoothing(new CSSSmoothing())
{
}

CVSystem::Triangulator1::~Triangulator1()
{
	if(_trace_state)  potrace_state_free(_trace_state);
	if(_cssSmoothing) delete _cssSmoothing;
}

void CVSystem::Triangulator1::ReadFromMYOBJ(std::string filename)
{
	_offsets.resize(8);

	OBJIO::ReadFromMYOBJ(filename,
						_w_scaled,
						_h_scaled,
						_offsets,
						_vertexList,			// The actual vertices
						_indexedTriangles,		// All triangles
						_borderSCTriangles,		// Additional screentone triangles
						_borderWTriangles,		// Additional white triangles
						_scTriangles,			// Screentone triangles
						_wTriangles,			// White triangles
						_bTriangles,			// Black triangles
						_indexedBezierCurves,	// Bezier curves (start and end points only)
						_lsQuads);	
		
	SystemParams::t_scale_factor = 1.0;
}

void CVSystem::Triangulator1::TraceImage(std::string strFilename, cv::Mat img, int* mask, int* dilatedMask)
{
	// Timing calculation
	using namespace boost::chrono ;
	auto start = steady_clock::now() ;

	// filename
	this->_strFilename = strFilename;


	std::cout << "Initializing Triangulation\n";
	//this->Reset();
	//_cssSmoothing = new CSSSmoothing();

	// Masking if available
	if(mask)
	{
		this->_has_tone = true;
		cv::Mat imgMaskDilated = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
		for(int a = 0; a < imgMaskDilated.cols; a++)
		{
			for(int b = 0; b < imgMaskDilated.rows; b++)
				{ if(dilatedMask[a + b * img.cols] != -1) { imgMaskDilated.ptr<uchar>(b, a)[0] = 255; } }
		}

		cv::Mat imgMask = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
		for(int a = 0; a < imgMask.cols; a++)
		{
			for(int b = 0; b < imgMask.rows; b++)
				{ if(mask[a + b * img.cols] != -1) { imgMask.ptr<uchar>(b, a)[0] = 255; } }
		}

		cv::Mat invMaskImg;
		cv::bitwise_not(imgMask, invMaskImg);
		cv::distanceTransform(invMaskImg, this->_distance_img, CV_DIST_L2, CV_DIST_MASK_5);

		this->_mask = imgMask.clone();
		this->_dilated_mask = imgMaskDilated.clone();

		img += imgMask;
	}

	// Dimension
	this->_w_scaled = img.cols * SystemParams::t_scale_factor;
	this->_h_scaled = img.rows * SystemParams::t_scale_factor;
	cv::Size sz(_w_scaled, _h_scaled);

	if(this->_has_tone)
	{
		cv::resize(this->_mask,			this->_mask_scaled,		    sz);
		cv::resize(this->_dilated_mask, this->_dilated_mask_scaled, sz);
		cv::resize(this->_distance_img, this->_distance_img_scaled, sz);
	}

	cv::Mat inpImg = img.clone();	
	cv::resize(inpImg, inpImg, sz);

	potrace_bitmap_t* bm = GetBM(inpImg);

	// Tracing
	potrace_param_t* param   =  potrace_param_default();
	param->opticurve         = SystemParams::t_opticurve;
	param->opttolerance      = SystemParams::t_opttolerance;
	param->progress.callback = NULL;
	_trace_state			 = potrace_trace(param, bm);

	// Free resources
	potrace_param_free(param);
	free(bm->map);
	free(bm);

	auto dur = steady_clock::now() - start ;
	std::cout << "Tracing time: " << duration_cast<milliseconds>(dur).count() << " milliseconds\n" ;
}

void CVSystem::Triangulator1::CleanUpPoly(std::vector<CVSystem::MyPoint>& poly)
{
	std::vector<CVSystem::MyPoint> tempPoly;
	for(size_t a = 0; a < poly.size(); a++)
		{ tempPoly.push_back(CVSystem::MyPoint(poly[a].x, poly[a].y)); }

	poly.clear();	
	poly.push_back(tempPoly[0]);
	for(size_t a = 1; a < tempPoly.size(); a++)
	{ if(tempPoly[a] != tempPoly[a - 1])
			{ poly.push_back(tempPoly[a]); } }

	// get size
	std::vector<cv::Point> contour;
	for(size_t b = 0; b < poly.size(); b++)
		{ contour.push_back(cv::Point2d(poly[b].x, poly[b].y));} 
	double polyArea = cv::contourArea(contour);
		
	if(polyArea < SystemParams::t_min_poly_area * SystemParams::t_scale_factor)
	{ /*std::cout << "poly_cleared ";*/ poly.clear(); }
}

std::list<CVSystem::CD_Point> CVSystem::Triangulator1::GetHoleSeeds(std::vector<CVSystem::MyPoint> pPoly, std::vector<std::vector<CVSystem::MyPoint>> cPoly)
{
	std::list<CD_Point> seeds;

	// (PD) for outer polygon
	PD_Cdt pd_cdt;
	Polygon_2 polygon1;		

	for(size_t a = 0; a < pPoly.size() - 1; a++)
		{ polygon1.push_back(PD_Point(pPoly[a].x, pPoly[a].y)); } 
	insert_polygon(pd_cdt, polygon1);		

	// (PD )for inner polygon
	for(size_t a = 0; a < cPoly.size(); a++)
	{
		Polygon_2 polygon2;
		std::vector<CVSystem::MyPoint> cPs = cPoly[a];
		for(size_t b = 0; b < cPs.size() - 1; b++)
			{ polygon2.push_back(PD_Point(cPs[b].x, cPs[b].y)); }
		insert_polygon(pd_cdt, polygon2);
	}

	// (PD) Mark domains
	mark_domains(pd_cdt);

	// (PD) Get holes
	for (PD_Cdt::Finite_faces_iterator fit = pd_cdt.finite_faces_begin(); fit != pd_cdt.finite_faces_end(); ++fit)
	{	
		if ( !fit->info().in_domain() )
		{
			float x_center = ((*(fit->vertex(0))).point().x() + (*(fit->vertex(1))).point().x() + (*(fit->vertex(2))).point().x()) / 3.0;
			float y_center = ((*(fit->vertex(0))).point().y() + (*(fit->vertex(1))).point().y() + (*(fit->vertex(2))).point().y()) / 3.0;
			seeds.push_back(CD_Point(x_center, y_center));
		}
	}
	return seeds;
}

// Todo: known bug: doesn't work for blurred image? 
// black triangles become white triangle ???
void CVSystem::Triangulator1::LSCalculate2()
{
	// Timing calculation
	using namespace boost::chrono ;
	auto total_smoothing_time = 0;
	auto ls_time = 0;

	std::vector<MyTriangle>			  scTriangles;
	std::vector<MyTriangle>			  blackTriangles;
	std::vector<std::vector<MyPoint>> allPolygons;

	// Get Seed
	for (potrace_path_t* p = _trace_state->plist; p; p = p->next)
	{
		if(p->sign == '-') continue;

		std::vector<MyPoint> pPolySimple;

		// Timing
		auto startGauss1 = steady_clock::now() ;	

		std::vector<MyPoint> pPolyDense = GetPolygonLS(p);

		// Timing
		auto durGauss1 = steady_clock::now() - startGauss1 ;
		total_smoothing_time += duration_cast<milliseconds>(durGauss1).count();

		if(pPolyDense.size() == 0) { continue;	/* Something bad happens ! */ }

		std::vector<bool> pFlag;
		RDPSimplification(pPolyDense, pPolySimple, pFlag);
		if (pPolySimple.size() > 2) 
		{
			auto startLS1 = steady_clock::now() ;

			std::vector<MyQuad> tempQuads = GetBezierQuadLS(pPolyDense, pFlag, true);
			this->_lsQuads.insert(this->_lsQuads.begin(), tempQuads.begin(), tempQuads.end());

			auto durLS1 = steady_clock::now() - startLS1 ;
			ls_time += duration_cast<milliseconds>(durLS1).count();
		}

		std::vector<std::vector<MyPoint>> cPolySimple;
		for (potrace_path_t* q = p->childlist; q; q = q->sibling)
		{
			std::vector<MyPoint> qPolyTempSimple;

			// Timing
			auto start2 = steady_clock::now() ;	

			std::vector<MyPoint> qPolyTempDense = GetPolygonLS(q);

			// Timing
			auto durGauss2 = steady_clock::now() - start2 ;
			total_smoothing_time += duration_cast<milliseconds>(durGauss2).count();

			if(qPolyTempDense.size() == 0) { continue; }
			std::vector<bool> qFlagTemp;
			RDPSimplification(qPolyTempDense, qPolyTempSimple, qFlagTemp);			
			if (qPolyTempSimple.size() > 2) 
			{
				// Timing
				auto startLS2 = steady_clock::now() ;

				std::vector<MyQuad> tempQuads = GetBezierQuadLS(qPolyTempDense, qFlagTemp, false);
				this->_lsQuads.insert(this->_lsQuads.begin(), tempQuads.begin(), tempQuads.end());
				cPolySimple.push_back(qPolyTempSimple);

				// Timing
				auto durLS2 = steady_clock::now() - startLS2 ;
				ls_time += duration_cast<milliseconds>(durLS2).count();
			}
		}

		if(pPolySimple.size() == 0) { continue;	/* Something bad happens */ }

		CD_Cdt cdt_blk = GetCDT(pPolySimple, cPolySimple);
		//Testing
		allPolygons.push_back(pPolySimple);
		for(size_t a = 0; a < cPolySimple.size(); a++) { allPolygons.push_back(cPolySimple[a]); }

		for(CD_Cdt::Finite_faces_iterator fit = cdt_blk.finite_faces_begin(); fit != cdt_blk.finite_faces_end(); ++fit) 
		{
			if(fit->is_in_domain()) 
			{
				MyTriangle tr = MyTriangle( MyPoint(fit->vertex(0)->point().x(), fit->vertex(0)->point().y()), 
											MyPoint(fit->vertex(1)->point().x(), fit->vertex(1)->point().y()), 
											MyPoint(fit->vertex(2)->point().x(), fit->vertex(2)->point().y()), 
											TRIANGLE_BLACK);
				blackTriangles.push_back(tr);
			}
		}		
	}

	// Timing
	std::cout << "SMoothing time: " << total_smoothing_time << " milliseconds\n";

	// Timing
	auto start = steady_clock::now() ;

	CD_Cdt cd_cdt1 = GetCDTWithRectangleBorder(allPolygons);	// conforming delaunay
	std::vector<CD_Cdt::Face_handle> fsh;
	if(this->_has_tone)
	{
		std::cout << "Calculate screentone borders...\n";
		
		for(CD_Cdt::Finite_faces_iterator fit = cd_cdt1.finite_faces_begin(); fit != cd_cdt1.finite_faces_end(); ++fit) 
		{
			MyTriangle tr = MyTriangle( MyPoint(fit->vertex(0)->point().x(), fit->vertex(0)->point().y()), 
										MyPoint(fit->vertex(1)->point().x(), fit->vertex(1)->point().y()), 
										MyPoint(fit->vertex(2)->point().x(), fit->vertex(2)->point().y()), 
										TRIANGLE_NOT_DEFINED);

			int pixelValue = PixelsInTriangles::MedianValue(this->_dilated_mask_scaled, tr.p1, tr.p2, tr.p3);
			MyPoint ctr = tr.GetCenter();

			if(pixelValue < 127  || this->_distance_img_scaled.ptr<float>(ctr.y, ctr.x)[0] > 30)
			{ 
				if(fit->is_marked()) {fsh.push_back(fit);}
				fit->set_marked(false); 				
			}
			else
			{
				tr.tri_type = TRIANGLE_SCREENTONE;
				scTriangles.push_back(tr);
			}
		}

		for (CD_Cdt::Finite_edges_iterator eit = cd_cdt1.finite_edges_begin(); eit != cd_cdt1.finite_edges_end(); ++eit) 
		{
			const CD_Cdt::Face_handle& fh = eit->first;

			int ctr = 0;
			if(fh->is_in_domain()) { ctr++; }
			if(fh->neighbor(eit->second)->is_in_domain()) { ctr++; }
			if(ctr == 1)
			{
				CD_Cdt::Face_handle fh = eit->first;
				int i = eit->second;
				cd_cdt1.insert_constraint(fh->vertex(fh->cw(i)), fh->vertex(fh->ccw(i)));
			}
		}

		// Rollback !
		for(size_t a = 0; a < fsh.size(); a++)
		{ fsh[a]->set_marked(false); }

		// A seed on the border
		std::list<CD_Point> seeds;
		seeds.push_back(CD_Point(1, 1));

		CGAL::refine_Delaunay_mesh_2(cd_cdt1, seeds.begin(), seeds.end(), CD_Criteria(SystemParams::t_delaunay_aspect_bound, SystemParams::t_delaunay_max_length));
		AssignVertexIndex(cd_cdt1);

		PointInTrianglesTest* bTest = new PointInTrianglesTest();
		bTest->PushTriangles(blackTriangles);

		PointInTrianglesTest* scTest = new PointInTrianglesTest();
		scTest->PushTriangles(scTriangles);
		
		for(CD_Cdt::Finite_faces_iterator fit = cd_cdt1.finite_faces_begin(); fit != cd_cdt1.finite_faces_end(); ++fit) 
		{
			if(fit->is_in_domain())
			{
				TriangleType triType = TRIANGLE_NOT_DEFINED;
				if(bTest->IsInside(fit))		{triType = TRIANGLE_BLACK;}
				else if(scTest->IsInside(fit))  {triType = TRIANGLE_SCREENTONE;}
				else							{triType = TRIANGLE_WHITE;}
				fit->triangleType = triType;

				MyIndexedTriangle tri( fit->vertex(0)->info(), fit->vertex(1)->info(), fit->vertex(2)->info(), triType);

				this->_indexedTriangles.push_back(tri);
				if     (triType == TRIANGLE_SCREENTONE) { this->_scTriangles.push_back(tri); }
				else if(triType == TRIANGLE_WHITE)		 { this->_wTriangles.push_back(tri); }
				else if(triType == TRIANGLE_BLACK)		 { this->_bTriangles.push_back(tri); }
			}
		}
		ProcessEdgesFromCDT(cd_cdt1);
	}
	else /* doesn't have screentone */
	{
		// A seed on the border
		std::list<CD_Point> seeds;
		seeds.push_back(CD_Point(1, 1));

		CGAL::refine_Delaunay_mesh_2(cd_cdt1, seeds.begin(), seeds.end(), CD_Criteria(SystemParams::t_delaunay_aspect_bound, SystemParams::t_delaunay_max_length));
		AssignVertexIndex(cd_cdt1);

		PointInTrianglesTest* bTest = new PointInTrianglesTest();
		bTest->PushTriangles(blackTriangles);

		for(CD_Cdt::Finite_faces_iterator fit = cd_cdt1.finite_faces_begin(); fit != cd_cdt1.finite_faces_end(); ++fit) 
		{
			if(fit->is_in_domain())
			{
				TriangleType triType = (bTest->IsInside(fit)) ? TRIANGLE_BLACK : TRIANGLE_WHITE;
				MyIndexedTriangle tri(fit->vertex(0)->info(), fit->vertex(1)->info(), fit->vertex(2)->info(), triType);
				this->_indexedTriangles.push_back(tri);
				fit->triangleType = triType;

				if(triType == TRIANGLE_WHITE) { this->_wTriangles.push_back(tri); }
				else if(triType == TRIANGLE_BLACK) { this->_bTriangles.push_back(tri); }
			}
		}

		ProcessEdgesFromCDT(cd_cdt1);
	}

	GetCornersFromPolygon();
	ProcessPolygonCorners();

	// Timing
	auto dur = steady_clock::now() - start ;
	std::cout << "least square time: " << ls_time << " milliseconds\n" ;
	std::cout << "triangulation time: " << duration_cast<milliseconds>(dur).count() << " milliseconds\n" ;

	std::vector<std::string> fullpath = UtilityFunctions::split(_strFilename, '//');
	std::vector<std::string> nameArray = UtilityFunctions::split(fullpath[fullpath.size() - 1], '.');
	std::string meshFilename = "Mesh/" + nameArray[0] + ".myobj";

	std::vector<MyQuad> resizedLsQuads = this->GetLSQuads();

	_offsets.resize(8);
	for(size_t a = 0; a < _offsets.size(); a++)
		{ _offsets[a] = 0; }

	OBJIO::WriteMyToOBJ(meshFilename, 
						_w_scaled / SystemParams::t_scale_factor,
						_h_scaled / SystemParams::t_scale_factor,
						_offsets,
						_vertexList, 
						_indexedBezierCurves,
						_borderSCTriangles,
						_borderWTriangles,
						resizedLsQuads,		// resized
						_indexedTriangles);

	//OBJWriter::WriteToOBJ("file.obj", cd_cdt1);	
	//MakeIndexedTriangles(cd_cdt1);

	std::cout << "Done\n";
}

void  CVSystem::Triangulator1::ProcessEdgesFromCDT(CVSystem::CD_Cdt cd_cdt)
{
	this->_borderSCTriangles.clear();
	this->_borderWTriangles.clear();

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

		// should be flipped so black triangles are in left side
		if(fh1->triangleType != TRIANGLE_BLACK && fh2->triangleType == TRIANGLE_BLACK)
		{
			int i = eit->second;					
			int index0 = fh1->vertex(fh1->cw(i))->info();
			int index1 = fh1->vertex(fh1->ccw(i))->info();

			unsortedLines.push_back(MyIndexedLine(index1, index0));
			//unsortedLines.push_back(MyLine(x2, y2, x1, y1, index1, index0));
		}
		
		if(fh1->triangleType == TRIANGLE_BLACK && fh2->triangleType == TRIANGLE_WHITE)		   
			{ this->_borderWTriangles.push_back( MyIndexedTriangle( fh1->vertex(0)->info(), fh1->vertex(1)->info(), fh1->vertex(2)->info(), TRIANGLE_WHITE)); }
		else if(fh1->triangleType == TRIANGLE_WHITE && fh2->triangleType == TRIANGLE_BLACK)
			{ this->_borderWTriangles.push_back( MyIndexedTriangle( fh2->vertex(0)->info(), fh2->vertex(1)->info(), fh2->vertex(2)->info(), TRIANGLE_WHITE)); }
		else if(fh1->triangleType == TRIANGLE_BLACK && fh2->triangleType == TRIANGLE_SCREENTONE)
			{ this->_borderSCTriangles.push_back( MyIndexedTriangle( fh1->vertex(0)->info(), fh1->vertex(1)->info(), fh1->vertex(2)->info(), TRIANGLE_SCREENTONE)); }
		else if(fh1->triangleType == TRIANGLE_SCREENTONE && fh2->triangleType == TRIANGLE_BLACK)
			{ this->_borderSCTriangles.push_back( MyIndexedTriangle( fh2->vertex(0)->info(), fh2->vertex(1)->info(),  fh2->vertex(2)->info(), TRIANGLE_SCREENTONE)); }

	}

	this->_indexedBezierCurves = LinesSorter::BuildBezierCurves2(unsortedLines, this->_vertexList);
}

// Debugging

//void CVSystem::Triangulator::GetCornersDebug(std::vector<CVSystem::MyPoint> poly)
//{
	//this->_cornerList.push_back(poly[0]);
	//this->_cornerList.push_back(poly[poly.size() - 1]);
//}

void CVSystem::Triangulator1::ProcessPolygonCorners()
{
	// all corner flags are false
	for(size_t a = 0; a < this->_indexedBezierCurves.size(); a++)
	{
		MyIndexedBezierCurves* bCurve = &this->_indexedBezierCurves[a];
		bCurve->cornerFlags.resize(bCurve->indices.size());
		for(size_t b = 0; b < bCurve->cornerFlags.size(); b++)
			{ bCurve->cornerFlags[b] = false; }
	}

	// Todo: put this in SystemParams
	double cornerAngle = 2.792526803; // 160
	for(size_t a = 0; a < this->_indexedBezierCurves.size(); a++)
	{
		MyIndexedBezierCurves* bCurve = &this->_indexedBezierCurves[a];
		for(size_t b = 0; b < bCurve->indices.size() - 1; b++)
		{
			MyPoint p0;
			if(b == 0) { p0 = _vertexList[bCurve->indices[bCurve->indices.size() - 2]]; }
			else  { p0 = _vertexList[bCurve->indices[b - 1]]; }

			MyPoint p1 = _vertexList[bCurve->indices[b]];
			MyPoint p2 = _vertexList[bCurve->indices[b + 1]];
			MyPoint dir0 = p0 - p1;
			MyPoint dir1 = p2 - p1;

			double angle = acos(dir0.Dot(dir1) / (dir0.Length() * dir1.Length()));

			if(angle < cornerAngle) { bCurve->cornerFlags[b] = true; }
		}
	}

	for(size_t a = 0; a < this->_indexedBezierCurves.size(); a++)
	{
		MyIndexedBezierCurves bCurve = this->_indexedBezierCurves[a];
		for(size_t b = 0; b < bCurve.indices.size() - 1; b++)
		{
			if(bCurve.cornerFlags[b])
			{
				int prevIndex = -1;
				int nextIndex = -1;
				if(b == 0) { prevIndex = bCurve.indices.size() - 2; }
				else	   { prevIndex = b - 1; }

				if(b == bCurve.indices.size() - 2) {nextIndex = 0;}
				else {nextIndex = b + 1;}

				// always take previous
				_cornerEdges.push_back(MyIndexedLine(bCurve.indices[prevIndex], bCurve.indices[b]));

				// if next is also corner, don't take that
				if(!bCurve.cornerFlags[nextIndex])
				{ _cornerEdges.push_back(MyIndexedLine(bCurve.indices[b], bCurve.indices[nextIndex])); }

			}
		}
	}
}

void CVSystem::Triangulator1::GetCornersFromPolygon()
{
	double cornerAngle = 2.792526803; // 160
	for(size_t a = 0; a < this->_indexedBezierCurves.size(); a++)
	{
		MyIndexedBezierCurves bCurve = this->_indexedBezierCurves[a];
		//this->_cornerList.push_back(poly.points[0]);		
		for(size_t b = 0; b < bCurve.indices.size() - 1; b++)
		{
			MyPoint p0;
			if(b == 0) { p0 = _vertexList[bCurve.indices[bCurve.indices.size() - 2]]; }
			else  { p0 = _vertexList[bCurve.indices[b - 1]]; }

			MyPoint p1 = _vertexList[bCurve.indices[b]];
			MyPoint p2 = _vertexList[bCurve.indices[b + 1]];
			MyPoint dir0 = p0 - p1;
			MyPoint dir1 = p2 - p1;

			//cos(angle) = dot_product / (a.len * b.len) 
			double angle = acos(dir0.Dot(dir1) / (dir0.Length() * dir1.Length()));

			if(angle < cornerAngle) 
			{
				this->_cornerList.push_back(p1);	// Fucking delete this
				this->_cornerIndices.push_back(bCurve.indices[b]);
			}
		}
	}
}

// Add index on vertices similar to .OBJ file format
void CVSystem::Triangulator1::AssignVertexIndex(CVSystem::CD_Cdt& cdt)
{
	// Make all invalid
	for (CD_Cdt::Finite_vertices_iterator vit = cdt.finite_vertices_begin(); vit != cdt.finite_vertices_end(); ++vit) 
		{ vit->info() = -2; }

	for(CD_Cdt::Finite_faces_iterator fit = cdt.finite_faces_begin(); fit != cdt.finite_faces_end(); ++fit) 
		{ if(fit->is_in_domain()) { fit->vertex(0)->info() = -1; fit->vertex(1)->info() = -1; fit->vertex(2)->info() = -1; } }

	// Start assigning index
	int idxCounter = 0;
	for (CD_Cdt::Finite_vertices_iterator vit = cdt.finite_vertices_begin(); vit != cdt.finite_vertices_end(); ++vit) 
		{ if(vit->info() == -1) { vit->info() = idxCounter++; } }

	// Add indexed vertices to list
	this->_vertexList.clear();
	for (CD_Cdt::Finite_vertices_iterator vit = cdt.finite_vertices_begin(); vit != cdt.finite_vertices_end(); ++vit) 
		{ if(vit->info() >= 0) { this->_vertexList.push_back(MyPoint(vit->point().x(), vit->point().y())); } }
}


void CVSystem::Triangulator1::RDPSimplification(std::vector<CVSystem::MyPoint> poly, std::vector<CVSystem::MyPoint>& rdpPoly, std::vector<bool>& bFlag)
{
	bFlag = std::vector<bool>(poly.size());
	for(size_t a = 0; a < poly.size(); a++)  { bFlag[a] = false; }
	bFlag[0] = true;
	bFlag[poly.size() - 1] = true;

	CurveRDP::RDP(bFlag, poly, SystemParams::t_rdp_epsilon, 0, poly.size() - 1, SystemParams::t_rdp_point_min);	
	for(size_t a = 0; a < poly.size(); a++)
		{ if(bFlag[a]) rdpPoly.push_back(poly[a]); }

	//CleanUpPoly(rdpPoly);
}


void CVSystem::Triangulator1::Resubdivide(std::vector<CVSystem::MyPoint>& poly)
{
	using namespace CVSystem;
	std::vector<CVSystem::MyPoint> tempPoly;
	for(size_t a = 0; a < poly.size(); a++)
		{ tempPoly.push_back(MyPoint(poly[a].x, poly[a].y)); }

	poly.clear();

	// Warning, hard parameters, but work just fine right now...
	double factr = 0.25;
	if(SystemParams::t_subdivide_limit > 5.0) { factr = 0.01; }

	for(size_t a = 0; a < tempPoly.size() - 1; a++)
	{
		CurveInterpolation::PointInterpolation(poly, tempPoly[a], tempPoly[a+1], 0.5, SystemParams::t_subdivide_limit * factr);
		poly.push_back(tempPoly[a+1]);
	}
}

std::vector<CVSystem::MyQuad> CVSystem::Triangulator1::GetBezierQuadLS(std::vector<CVSystem::MyPoint> poly, std::vector<bool> rdpFlags, bool isInside)
{
	std::vector<MyQuad> quads;

	// Begin to sample points
	std::vector<std::vector<MyPoint>> rdpPoly;
	rdpPoly.push_back(std::vector<CVSystem::MyPoint>());

	//MyPoint firstPt = poly[0];
	//MyPoint lastPt = poly[poly.size() - 1];

	for(size_t a = 0; a < poly.size(); a++)
	{
		int idx = rdpPoly.size() - 1;
		rdpPoly[idx].push_back(poly[a]);
		if(rdpFlags[a] && a != 0 && a != poly.size() - 1)  
		{
			rdpPoly.push_back( std::vector<MyPoint>() );
			rdpPoly[idx + 1].push_back(poly[a]);
		}
	}

	for(size_t a = 0; a < rdpPoly.size(); a++)
	{
		std::vector<MyPoint> segm = rdpPoly[a];

		// RESUBDIVIDE
		if(segm.size() < 5) { Resubdivide(segm); }

		std::vector<MyPoint> fourPoints;
		if( CurveFitting::PointstoBezier(segm, fourPoints))
			{ quads.push_back( MyQuad(segm[0],  fourPoints[1], fourPoints[2], segm[segm.size() - 1])); }
		else { }
	}

	// testing for c1 continuity
	/*MyQuad q0 = quads[0];
	MyQuad qN = quads[quads.size() - 1];

	std::cout << q0.p0.x << " " << q0.p0.y << "\n";
	std::cout << qN.p3.x << " " << qN.p3.y << "\n";
	std::cout << "\n";*/
	
	// Todo: enforce non-corners to be c1
	/*
	for(size_t a = 0; a < quads.size(); a++)
	{
		MyQuad* q1 = &quads[a];
		MyQuad* q2 = 0;
		if(a < quads.size() - 1) {q2 = &quads[a+1];}
		else {q2 = &quads[0];}

		// modify q1 p3
		// modify q2 p0

		MyPoint midPoint = (q1->p2 + q2->p1) / 2.0;
		q1->p3 = midPoint;
		q2->p0 = midPoint;

	}
	*/
	return quads;
}


CVSystem::CD_Cdt CVSystem::Triangulator1::GetCDT(std::vector<CVSystem::MyPoint> pPoly, std::vector<std::vector<CVSystem::MyPoint>> cPoly, bool shouldRefine)
{
	CD_Cdt cd_cdt;
	std::vector<std::vector<CD_VHandle>> handles;

	std::vector<CD_VHandle> hdls1;
	for(size_t a = 0; a < pPoly.size() - 1; a++)
		{ hdls1.push_back(cd_cdt.insert(CD_Point(pPoly[a].x, pPoly[a].y))); }
	handles.push_back(hdls1);

	if(cPoly.size() > 0)
	{
		for(size_t a = 0; a < cPoly.size(); a++)
		{
			std::vector<CVSystem::MyPoint> cPs = cPoly[a];
			std::vector<CD_VHandle> hdls2;
			for(size_t b = 0; b < cPs.size() - 1; b++) 
				{ hdls2.push_back( cd_cdt.insert(CD_Point(cPs[b].x, cPs[b].y)) ); }
			handles.push_back(hdls2);
		}
	}

	// Get holes
	std::list<CD_Point> seeds = GetHoleSeeds(pPoly, cPoly);

	// set constraints
	for(size_t a = 0; a < handles.size(); a++)
	{
		std::vector<CD_VHandle> hdls = handles[a];
		for(size_t b = 0; b < hdls.size(); b++)
		{
			if(b == 0) { cd_cdt.insert_constraint(hdls[hdls.size() - 1], hdls[b]); }
			else	   { cd_cdt.insert_constraint(hdls[b-1], hdls[b]); }
		}
	}

	if(shouldRefine) { CGAL::refine_Delaunay_mesh_2(cd_cdt, seeds.begin(), seeds.end(), CD_Criteria(SystemParams::t_delaunay_aspect_bound, SystemParams::t_delaunay_max_length)); }
	else
		{	// use small factor to obtain less Delaunay
			CGAL::refine_Delaunay_mesh_2(cd_cdt, seeds.begin(), seeds.end(), CD_Criteria(0.01, SystemParams::t_delaunay_max_length)); }

	return cd_cdt;
}

// Is always refined to be as delaunay as possible
// This function is only called to obtain full mesh of manga characters
CVSystem::CD_Cdt CVSystem::Triangulator1::GetCDTWithRectangleBorder(std::vector<std::vector<MyPoint>> polygons)
{
	CD_Cdt cd_cdt;
	std::vector<std::vector<CD_VHandle>> vHandles;

	std::vector<MyPoint> border;
	border.push_back(MyPoint(0,					0));
	border.push_back(MyPoint(this->_w_scaled,	0));
	border.push_back(MyPoint(this->_w_scaled,	this->_h_scaled));
	border.push_back(MyPoint(0,					this->_h_scaled));

	//polygons.push_back(border);
	for(size_t a = 0; a < polygons.size(); a++)
	{
		std::vector<CVSystem::MyPoint> cPs = polygons[a];
		std::vector<CD_VHandle> hdls;
		for(size_t b = 0; b < cPs.size() - 1; b++)  { hdls.push_back( cd_cdt.insert(CD_Point(cPs[b].x, cPs[b].y)) );  }
		vHandles.push_back(hdls);
	}

	// For image border only
	std::vector<CD_VHandle> borderVHandles;
	for(size_t a = 0; a < border.size(); a++) { borderVHandles.push_back( cd_cdt.insert(CD_Point(border[a].x, border[a].y)) );  }
	vHandles.push_back(borderVHandles);
	for(size_t a = 0; a < vHandles.size(); a++)
	{
		std::vector<CD_VHandle> hdls = vHandles[a];
		for(size_t b = 0; b < hdls.size(); b++)
		{
			if(b == 0) { cd_cdt.insert_constraint(hdls[hdls.size() - 1], hdls[b]); }
			else { cd_cdt.insert_constraint(hdls[b-1], hdls[b]); }
		}
	}
	CGAL::refine_Delaunay_mesh_2(cd_cdt, CD_Criteria(SystemParams::t_delaunay_aspect_bound, SystemParams::t_delaunay_max_length));
	return cd_cdt;
}


std::vector<CVSystem::MyPoint> CVSystem::Triangulator1::GetPolygonLS(potrace_path_t*  path)
{
	potrace_curve_t curve = path->curve;
	std::vector<MyPoint> poly;

	for(int a = 0; a < curve.n; a++)
	{
		MyPoint p0; MyPoint p1; MyPoint p2; MyPoint p3;
		
		if( a == 0) { p0.x = curve.c[curve.n-1][2].x; p0.y = this->_h_scaled - curve.c[curve.n-1][2].y; }
		else        { p0.x = curve.c[a-1][2].x;       p0.y = this->_h_scaled - curve.c[a-1][2].y; }
		
		p3 = MyPoint(curve.c[a][2].x, this->_h_scaled - curve.c[a][2].y);

		if(curve.tag[a] == POTRACE_CORNER)
		{
			MyPoint pMid(curve.c[a][1].x, this->_h_scaled - curve.c[a][1].y);

			CurveInterpolation::PointInterpolation(poly, p0, pMid, 0.5, SystemParams::t_subdivide_limit);
			poly.push_back(pMid);
			CurveInterpolation::PointInterpolation(poly, pMid, p3, 0.5, SystemParams::t_subdivide_limit);
		}
		else
		{
			p1 = MyPoint(curve.c[a][0].x, this->_h_scaled - curve.c[a][0].y);
			p2 = MyPoint(curve.c[a][1].x, this->_h_scaled - curve.c[a][1].y);

			CurveInterpolation::DeCasteljau(poly, p0, p1, p2, p3, SystemParams::t_subdivide_limit);
		}

		if(a == curve.n - 1) { poly.push_back(p3); }
	}

	if(this->_has_tone) { _cssSmoothing->SmoothCurve2(poly, this->_distance_img_scaled); }

	CleanUpPoly(poly);

	// !!! bug here...
	if(poly.size() > 2)
	{
		double dist = poly[0].Distance(poly[poly.size() - 1]);
		if(dist > M_EPS) { poly.push_back(poly[0]); }
	}

	return poly;
}

void CVSystem::Triangulator1::CurveToBezier(MyPoint p0, MyPoint p1, MyPoint p2, MyPoint p3, MyPoint& cp0, MyPoint& cp1)
{
	double xc1 = (p0.x + p1.x) / 2.0;		double yc1 = (p0.y + p1.y) / 2.0;
	double xc2 = (p1.x + p2.x) / 2.0;		double yc2 = (p1.y + p2.y) / 2.0;
	double xc3 = (p2.x + p3.x) / 2.0;		double yc3 = (p2.y + p3.y) / 2.0;

	double len1 = sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
	double len2 = sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
	double len3 = sqrt((p3.x - p2.x) * (p3.x - p2.x) + (p3.y - p2.y) * (p3.y - p2.y));

	double k1 = len1 / (len1 + len2);		double k2 = len2 / (len2 + len3);

	double xm1 = xc1 + (xc2 - xc1) * k1;	double ym1 = yc1 + (yc2 - yc1) * k1;
	double xm2 = xc2 + (xc3 - xc2) * k2;	double ym2 = yc2 + (yc3 - yc2) * k2;

	// Resulting control points. Here smooth_value is mentioned
	// above coefficient K whose value should be in range [0...1].
	cp0.x = xm1 + (xc2 - xm1) * SystemParams::t_smooth_factor + p1.x - xm1;
	cp0.y = ym1 + (yc2 - ym1) * SystemParams::t_smooth_factor + p1.y - ym1;

	cp1.x = xm2 + (xc2 - xm2) * SystemParams::t_smooth_factor + p2.x - xm2;
	cp1.y = ym2 + (yc2 - ym2) * SystemParams::t_smooth_factor + p2.y - ym2;
}

potrace_bitmap_t* CVSystem::Triangulator1::GetBM(cv::Mat img)
{
	cv::Mat flipImg;
	cv::flip(img, flipImg, 0);
	cv::Mat inpImg;
	flipImg.convertTo(inpImg, CV_8UC1);

	potrace_bitmap_t* bm = bm_new(img.cols, img.rows);
	for(int a = 0; a < bm->w; a++)
	{
		for(int b = 0; b < bm->h; b++)
		{
			// Todo: users should be able to set the threshold
			if(inpImg.ptr<uchar>(b, a)[0] < 127) { BM_PUT(bm, a, b, 1); }
			else { BM_PUT(bm, a, b, 0); }
		}
	}
	return bm;
}

// LS
std::vector<CVSystem::MyQuad> CVSystem::Triangulator1::GetLSQuads() 
{
	double val = 1.0 / SystemParams::t_scale_factor;
	std::vector<CVSystem::MyQuad> resizedLSQuads;
	for(size_t a = 0; a < _lsQuads.size(); a++)
		{ resizedLSQuads.push_back(_lsQuads[a].Resize(val)); }
	return resizedLSQuads;
}


std::vector<CVSystem::MyPoint> CVSystem::Triangulator1::GetVertexList()
{
	double val = 1.0 / SystemParams::t_scale_factor;
	std::vector<CVSystem::MyPoint>	resizedVertexList;
	for(size_t a = 0; a < this->_vertexList.size(); a++)
		{ resizedVertexList.push_back(this->_vertexList[a].Resize(val)); }
	return resizedVertexList;
}

std::vector<CVSystem::MyIndexedTriangle> CVSystem::Triangulator1::GetIndexedTriangles()
{ return this->_indexedTriangles; }

std::vector<CVSystem::MyIndexedTriangle> CVSystem::Triangulator1::GetBorderWTriangles()
{ return this->_borderWTriangles; }

std::vector<CVSystem::MyIndexedTriangle> CVSystem::Triangulator1::GetBorderSCTriangles()
{ return this->_borderSCTriangles; }

std::vector<CVSystem::MyIndexedTriangle> CVSystem::Triangulator1::GetWTriangles()
{ return this->_wTriangles; }

std::vector<CVSystem::MyIndexedTriangle> CVSystem::Triangulator1::GetSCTriangles()
{ return this->_scTriangles; }

std::vector<CVSystem::MyIndexedBezierCurves> CVSystem::Triangulator1::GetIndexedBezierCurves()
{
	for(size_t a = 0; a < _indexedBezierCurves.size(); a++)
	{
		_indexedBezierCurves[a].CalculateOrientation(_vertexList);
	}

	return this->_indexedBezierCurves; 
}

std::vector<CVSystem::MyIndexedTriangle> CVSystem::Triangulator1::GetBTriangles()
{ return this->_bTriangles; }

std::vector<int> CVSystem::Triangulator1::GetCornerIndices()
{ return this->_cornerIndices; }

std::vector<CVSystem::MyIndexedLine> CVSystem::Triangulator1::GetCornerEdges()
{ return this->_cornerEdges; }

// Deprecated
// Get corner points
std::vector<CVSystem::MyPoint> CVSystem::Triangulator1::GetCornerList()
{
	double val = 1.0 / SystemParams::t_scale_factor;
	std::vector<CVSystem::MyPoint>	resizedCornerList;
	for(size_t a = 0; a < this->_cornerList.size(); a++)
	{ resizedCornerList.push_back(this->_cornerList[a].Resize(val)); }
	return resizedCornerList;
}

// Deprecated, don't use this function
// Original Triangulation
void CVSystem::Triangulator1::OCalculate()
{
	// Get Seed
	potrace_path_t* p; potrace_path_t* q;
	for (p = _trace_state->plist; p; p = p->next)
	{
		if(p->sign == '-') { continue; }	// only look for positive one

		// PD
		std::vector<MyPoint> outerPoly;
		std::vector<std::vector<MyPoint>> childrenPoly;

		outerPoly = GetPolygonO(p->curve);
		for (q=p->childlist; q; q=q->sibling) { childrenPoly.push_back(GetPolygonO(q->curve)); }

		// Real quads here...
		std::vector<MyQuad> tempQuads1 = GetBezierQuad0(p);
		this->_oQuads.insert(this->_oQuads.end(), tempQuads1.begin(), tempQuads1.end());
		for (q=p->childlist; q; q=q->sibling) 
		{
			std::vector<MyQuad> tempQuads2 = GetBezierQuad0(q);
			this->_oQuads.insert(this->_oQuads.end(), tempQuads2.begin(), tempQuads2.end());
		}

		CD_Cdt cd_cdt = this->GetCDT(outerPoly, childrenPoly);
		for(CD_Cdt::Finite_faces_iterator fit = cd_cdt.finite_faces_begin(); fit != cd_cdt.finite_faces_end(); ++fit) 
		{
			if(fit->is_in_domain()) 
			{
				cv::Point2f pt0( (*(fit->vertex(0))).point().x(), (*(fit->vertex(0))).point().y() );
				cv::Point2f pt1( (*(fit->vertex(1))).point().x(), (*(fit->vertex(1))).point().y() );
				cv::Point2f pt2( (*(fit->vertex(2))).point().x(), (*(fit->vertex(2))).point().y() );
				MyTriangle tr(MyPoint(pt0.x, pt0.y), MyPoint(pt1.x, pt1.y), MyPoint(pt2.x, pt2.y), TRIANGLE_BLACK);
				_oTriangles.push_back(tr);
			}
		}		
	}
}

// Deprecated, don't use this function
// Least Square Triangulation
void CVSystem::Triangulator1::LSCalculate1()
{	
	int numVertex = 0;
	// Get Seed
	potrace_path_t* p; potrace_path_t* q;
	for (p = _trace_state->plist; p; p = p->next)
	{
		if(p->sign == '-') continue;

		std::vector<MyPoint> pPolySimple;
		std::vector<MyPoint> pPolyDense = GetPolygonLS(p);
		if(pPolyDense.size() == 0) { /* Something bad happens ! */ continue; }

		std::vector<bool> pFlag;
		RDPSimplification(pPolyDense, pPolySimple, pFlag);
		if (pPolySimple.size() > 2) 
		{
			std::vector<MyQuad> tempQuads = GetBezierQuadLS(pPolyDense, pFlag, true);
			this->_lsQuads.insert(this->_lsQuads.begin(), tempQuads.begin(), tempQuads.end());
		}
		std::vector<std::vector<MyPoint>> cPolySimple;
		for (q=p->childlist; q; q=q->sibling)
		{
			std::vector<MyPoint> qPolyTempSimple;
			std::vector<MyPoint> qPolyTempDense = GetPolygonLS(q);
			if(qPolyTempDense.size() == 0) { continue; }
			std::vector<bool> qFlagTemp;
			RDPSimplification(qPolyTempDense, qPolyTempSimple, qFlagTemp);			
			if (qPolyTempSimple.size() > 2) 
			{
				std::vector<MyQuad> tempQuads = GetBezierQuadLS(qPolyTempDense, qFlagTemp, false);
				this->_lsQuads.insert(this->_lsQuads.begin(), tempQuads.begin(), tempQuads.end());
				cPolySimple.push_back(qPolyTempSimple);
			}
		}

		if(pPolySimple.size() == 0) { /* Something bad happens ! */ continue; }

		CD_Cdt cdt = GetCDT(pPolySimple, cPolySimple);		
		for(CD_Cdt::Finite_faces_iterator fit = cdt.finite_faces_begin(); fit != cdt.finite_faces_end(); ++fit) 
		{
			MyTriangle tr = MyTriangle( MyPoint((*(fit->vertex(0))).point().x(), (*(fit->vertex(0))).point().y()), 
				MyPoint((*(fit->vertex(1))).point().x(), (*(fit->vertex(1))).point().y()), 
				MyPoint((*(fit->vertex(2))).point().x(), (*(fit->vertex(2))).point().y()), TRIANGLE_BLACK);

			if(fit->is_in_domain()) 
			{ _lsTriangles.push_back(tr); }
		}		


		numVertex += cdt.number_of_vertices();
	}

	std::cout << "Finished triangulation, n of vertices: " << numVertex << "\n";
	std::cout << "Finished triangulation, n of triangles: " << this->_lsTriangles.size() << "\n";
	std::cout << "Finished least square, n of bezier curve: " << this->_lsQuads.size() << "\n";	
}

// Deprecated
std::vector<CVSystem::MyPoint> CVSystem::Triangulator1::GetPolygonO(potrace_curve_t curve)
{
	std::vector<CVSystem::MyPoint> pts;
	for(int a = 0; a < curve.n; a++)
	{ 
		if(a == 0)  { pts.push_back(CVSystem::MyPoint(curve.c[curve.n-1][2].x, this->_h_scaled - curve.c[curve.n-1][2].y)); }
		else		{ pts.push_back(CVSystem::MyPoint(curve.c[a-1][2].x, this->_h_scaled - curve.c[a-1][2].y)); }

		if(curve.tag[a] == POTRACE_CORNER) { pts.push_back(CVSystem::MyPoint(curve.c[a][1].x, this->_h_scaled - curve.c[a][1].y)); }
	}
	return pts;
}

// Original (Deprecated)
std::vector<CVSystem::MyTriangle> CVSystem::Triangulator1::GetOTriangles() 
{ 
	double val = 1.0 / SystemParams::t_scale_factor;
	std::vector<CVSystem::MyTriangle> resizedOTriangles;
	for(size_t a = 0; a < _oTriangles.size(); a++)
	{ resizedOTriangles.push_back(_oTriangles[a].Resize(val)); }
	return resizedOTriangles;
}


// Original (Deprecated)
std::vector<CVSystem::MyQuad> CVSystem::Triangulator1::GetOQuads() 
{
	double val = 1.0 / SystemParams::t_scale_factor;
	std::vector<CVSystem::MyQuad> resizedOQuads;
	for(size_t a = 0; a < this->_oQuads.size(); a++)
	{ resizedOQuads.push_back(this->_oQuads[a].Resize(val)); }
	return resizedOQuads;
}


// LS (Deprecated)
std::vector<CVSystem::MyTriangle> CVSystem::Triangulator1::GetLSTriangles() 
{ 
	double val = 1.0 / SystemParams::t_scale_factor;
	std::vector<CVSystem::MyTriangle> resizedLSTriangles;
	for(size_t a = 0; a < _lsTriangles.size(); a++)
	{ resizedLSTriangles.push_back(_lsTriangles[a].Resize(val)); }
	return resizedLSTriangles;
}

// Deprecated
std::vector<CVSystem::MyQuad> CVSystem::Triangulator1::GetBezierQuad0(potrace_path_t*  path)
{
	std::vector<CVSystem::MyQuad> quads;
	potrace_curve_t curve = path->curve;
	bool isPlus = (path->sign == '-') ? false : true;

	// build polygon
	std::vector<CD_Point> points;
	for(int a = 0; a < curve.n; a++)
	{
		CD_Point p(curve.c[a][2].x, this->_h_scaled - curve.c[a][2].y);
		points.push_back(p);
	}

	for(int a = 0; a < curve.n; a++)
	{
		if(curve.tag[a] == POTRACE_CORNER) { }
		else // curve
		{
			MyPoint p0; MyPoint p1; MyPoint p2; MyPoint p3;

			MyPoint pPrev; MyPoint pNext;

			if( a == 0)
			{
				p0 = MyPoint(curve.c[curve.n-1][2].x, this->_h_scaled - curve.c[curve.n-1][2].y);
				pPrev = MyPoint(curve.c[curve.n-2][2].x, this->_h_scaled - curve.c[curve.n-2][2].y);
			}
			else
			{
				p0 = MyPoint(curve.c[a-1][2].x, this->_h_scaled - curve.c[a-1][2].y);
				if(a == 1)
				{ pPrev = MyPoint(curve.c[curve.n-1][2].x, this->_h_scaled - curve.c[curve.n-1][2].y); }
				else
				{ pPrev = MyPoint(curve.c[a-2][2].x, this->_h_scaled - curve.c[a-2][2].y); }
			}

			p3 = MyPoint(curve.c[a][2].x, this->_h_scaled - curve.c[a][2].y);
			if(a == curve.n - 1)
			{ pNext = MyPoint(curve.c[0][2].x, this->_h_scaled - curve.c[0][2].y); }
			else
			{ pNext = MyPoint(curve.c[a + 1][2].x, this->_h_scaled - curve.c[a + 1][2].y); }

			CurveToBezier(pPrev, p0, p3, pNext, p1, p2);

			// Using data from Potrace
			/*p1.x = curve.c[a][0].x;
			p1.y = h - curve.c[a][0].y;

			p2.x = curve.c[a][1].x;	  
			p2.y = h - curve.c[a][1].y;*/

			quads.push_back(CVSystem::MyQuad(p0, p1, p2, p3));
		}
	}
	return quads;
}

/*std::vector<CVSystem::MyPolygon> CVSystem::Triangulator::GetPolygonList()
{
	double val = 1.0 / SystemParams::t_scale_factor;
	std::vector<CVSystem::MyPolygon>	resizedPolygonList;
	for(size_t a = 0; a < _polygonList.size(); a++)
	{
		MyPolygon newPoly;
		MyPolygon oldPoly = _polygonList[a];
		for(size_t b = 0; b < oldPoly.points.size(); b++)
		{ newPoly.points.push_back(oldPoly.points[b].Resize(val)); }

		newPoly.SetRandomColor();
		newPoly.CalculateOrientation();
		resizedPolygonList.push_back(newPoly);
	}
	return resizedPolygonList;
}*/

/*
void CVSystem::Triangulator::Reset()
{
	this->_oTriangles.clear();
	this->_oQuads.clear();

	this->_lsQuads.clear();
	this->_lsTriangles.clear();
}
*/

/*
// this function should not be used anymore
void CVSystem::Triangulator::MakeIndexedTriangles(CVSystem::CD_Cdt cdt)
{
	this->_vertexList.clear();
	this->_indexedTriangles.clear();

	for (CD_Cdt::Finite_vertices_iterator vit = cdt.finite_vertices_begin(); vit != cdt.finite_vertices_end(); ++vit) 
	{ 
		if(vit->info() >= 0)
		{ this->_vertexList.push_back(MyPoint(vit->point().x(), vit->point().y())); }
	}

	for(CD_Cdt::Finite_faces_iterator fit = cdt.finite_faces_begin(); fit != cdt.finite_faces_end(); ++fit) 
	{
		if(fit->is_in_domain())
		{ this->_indexedTriangles.push_back(MyIndexedTriangles(fit->vertex(0)->info(), fit->vertex(1)->info(), fit->vertex(2)->info(), NOT_DEFINED)); }
	}
}*/

/*
std::vector<CVSystem::MyLine> CVSystem::Triangulator::GetLineDebug()
{
	double val = 1.0 / SystemParams::t_scale_factor;
	std::vector<CVSystem::MyLine> resizedLineDebug;
	for(size_t a = 0; a < _lineDebug.size(); a++)
		{ resizedLineDebug.push_back(_lineDebug[a].Resize(val)); }
	return resizedLineDebug;
}*/

/*
std::vector<CVSystem::MyTriangle> CVSystem::Triangulator::GetTriangleDebug()
{
	double val = 1.0 / SystemParams::t_scale_factor;
	std::vector<CVSystem::MyTriangle> resizedTriangleDebug;
	for(size_t a = 0; a < _triangleDebug.size(); a++)
		{ resizedTriangleDebug.push_back(_triangleDebug[a].Resize(val)); }
	return resizedTriangleDebug;
}*/

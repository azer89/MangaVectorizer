
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "PointLocationTest2.h"

CVSystem::PointLocationTest2::PointLocationTest2()
{
}

CVSystem::PointLocationTest2::~PointLocationTest2()
{
}


void CVSystem::PointLocationTest2::SetShapeSegments(std::vector<CVSystem::MyLine> lines)
{
	using namespace CVSystem;
	shapeSegments.clear();
	for(size_t a = 0; a < lines.size(); a++)
	{
		MyPoint ptA(lines[a].XA, lines[a].YA);
		MyPoint ptB(lines[a].XB, lines[a].YB);
		double dist = ptA.Distance(ptB);

		//// Bug here !!!
		if(dist < 1.3) {continue;}

		Segment_2 seg(Point_2(lines[a].XA, lines[a].YA), Point_2(lines[a].XB, lines[a].YB));
		shapeSegments.push_back(seg);
	}
}

std::vector<std::vector<CVSystem::MyPoint>> 
	CVSystem::PointLocationTest2::GetEnclosingPolygons(std::vector<CVSystem::MyLine> cutLines, 
													   CVSystem::MyPoint queryPt)
{
	Arrangement_2  arr;
	Walk_pl        pl(arr);

	std::vector<Segment_2> cutSegments;
	for(size_t a = 0; a < cutLines.size(); a++)
	{
		MyPoint ptA (cutLines[a].XA, cutLines[a].YA);
		MyPoint ptB(cutLines[a].XB, cutLines[a].YB);

		if(ptA.Distance(ptB) > M_EPS)
		{

		Segment_2 seg(Point_2(cutLines[a].XA, cutLines[a].YA), Point_2(cutLines[a].XB, cutLines[a].YB));
		cutSegments.push_back(seg);
		}
	}

	insert(arr, cutSegments.begin(), cutSegments.end());
	insert(arr, shapeSegments.begin(), shapeSegments.end());

	Point_2 q(queryPt.x, queryPt.y);
	Walk_pl::result_type obj = pl.locate(q);

	Arrangement_2::Face_const_handle  f;
	CGAL_assertion_code(bool success =) CGAL::assign(f, obj);
	CGAL_assertion(success);

	std::vector<std::vector<CVSystem::MyPoint>> resultPolygon = ExtractFace<Arrangement_2>(f);

	return resultPolygon;
}

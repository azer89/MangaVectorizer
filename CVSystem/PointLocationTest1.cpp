
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "PointLocationTest1.h"

#include <CGAL/Cartesian.h>
#include <CGAL/Quotient.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arr_walk_along_line_point_location.h>

#include "ArrangementExtractor.h"

/**
 *
 * Warning, the precision is integer
 * The resulting points are slightly misplaced but
 * the computation is faster.
 *
 * If you want floating number precision, change to 
 * CGAL::MP_Float
 *
 */

//typedef CGAL::Quotient<CGAL::MP_Float>						Number_type;
typedef CGAL::Quotient<int>										Number_type;

typedef CGAL::Cartesian<Number_type>                            Kernel;
typedef CGAL::Arr_segment_traits_2<Kernel>                      Traits_2;
typedef Traits_2::Point_2                                       Point_2;
typedef Traits_2::X_monotone_curve_2                            Segment_2;
typedef CGAL::Arrangement_2<Traits_2>                           Arrangement_2;
typedef CGAL::Arr_walk_along_line_point_location<Arrangement_2> Walk_pl;

std::vector<std::vector<CVSystem::MyPoint>> 
	CVSystem::PointLocationTest1::GetEnclosingPolygons(std::vector<CVSystem::MyLine>  lines, 
													  std::vector<CVSystem::MyLine>  cutLines, 
													  CVSystem::MyPoint  queryPt)
{
	Arrangement_2  arr;
	Walk_pl        pl(arr);
	
	//// Perform a point-location query on the resulting arrangement and print
	//// the boundary of the face that contains it.
	
	Point_2 q(queryPt.x, queryPt.y);
	Walk_pl::result_type obj = pl.locate(q);

	Arrangement_2::Face_const_handle  f;
	CGAL_assertion_code(bool success =) CGAL::assign(f, obj);
	CGAL_assertion(success);

	std::vector<std::vector<CVSystem::MyPoint>> resultPolygon = ExtractFace<Arrangement_2>(f);

	return resultPolygon;
}

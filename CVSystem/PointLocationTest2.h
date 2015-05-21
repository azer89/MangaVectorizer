
/**
 * Point location algorithm
 *
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#ifndef __Point_Location_Test_2__
#define __Point_Location_Test_2__

#include "MyPoint.h"
#include "MyLine.h"
#include <vector>

#include <CGAL/Cartesian.h>
#include <CGAL/Quotient.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arr_walk_along_line_point_location.h>

#include "ArrangementExtractor.h"

typedef CGAL::Quotient<int>										Number_type;

typedef CGAL::Cartesian<Number_type>                            Kernel;
typedef CGAL::Arr_segment_traits_2<Kernel>                      Traits_2;
typedef Traits_2::Point_2                                       Point_2;
typedef Traits_2::X_monotone_curve_2                            Segment_2;
typedef CGAL::Arrangement_2<Traits_2>                           Arrangement_2;
typedef CGAL::Arr_walk_along_line_point_location<Arrangement_2> Walk_pl;

namespace CVSystem
{
	class PointLocationTest2
	{
	public:
		// Constructor
		PointLocationTest2();

		// Destructor
		~PointLocationTest2();

		// get enclosing polygon
		std::vector<std::vector<MyPoint>> 
			GetEnclosingPolygons(std::vector<MyLine> cutLines, 
								 MyPoint queryPt);

		// set boundaries
		void SetShapeSegments(std::vector<MyLine> lines);

	private:
		// segments
		std::vector<Segment_2> shapeSegments;

	};
}

#endif

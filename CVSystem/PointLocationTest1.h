
/**
 * Point location algorithm
 *
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#ifndef __Point_Location_Test_1__
#define __Point_Location_Test_1__

#include "MyPoint.h"
#include "MyLine.h"
#include <vector>



namespace CVSystem
{
	class PointLocationTest1
	{
	public:
		// get enclosing polygon
		static std::vector<std::vector<MyPoint>> 
			GetEnclosingPolygons(std::vector<MyLine> lines, 
								 std::vector<MyLine> cutLines, 
								 MyPoint queryPt);

	};
}

#endif

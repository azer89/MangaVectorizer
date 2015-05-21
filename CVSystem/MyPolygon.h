
/**
 * Struct for polygon with hole(s)
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __My_Polygon__
#define __My_Polygon__

#include "MyLine.h"

namespace CVSystem
{
	struct MyPolygon
	{
	public:
		// outer boundary
		std::vector<MyLine> boundary;

		// inner boundaries
		std::vector<std::vector<MyLine>> holes;
	};
}

#endif
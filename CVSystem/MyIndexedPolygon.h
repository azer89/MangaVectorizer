
/**
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __My_Indexed_Polygon__
#define __My_Indexed_Polygon__

#include "MyPoint.h"
#include "MyIndexedLine.h"
#include <vector>

namespace CVSystem
{
	struct MyIndexedPolygon
	{
	public:
		//// Outer boundary
		std::vector<MyIndexedLine> boundary;

		//// Inner boundaries
		std::vector<std::vector<MyIndexedLine>> holes;

		//// Useful for re-triangulation
		std::vector<MyPoint> distinctPoints;

		//// Useful for re-triangulation
		std::vector<int> distinctIndices;
	};
}

#endif
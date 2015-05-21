
/**
 *
 * Triangles with three vertices
 * Each vertices doesn't hold the real coordinate
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __My_Indexed_Triangles__
#define __My_Indexed_Triangles__

#include "MyPoint.h"
#include "TriangleType.h"

namespace CVSystem
{
	
	struct MyIndexedTriangle
	{	
	public:
		// first index
		int idx0;

		// second index
		int idx1;

		// third index
		int idx2;

		// type of the triangle (black, white, and screentone)
		TriangleType tri_type;

		// Constructor
		MyIndexedTriangle(int idx0, int idx1, int idx2, TriangleType tri_type)
		{
			this->idx0 = idx0;
			this->idx1 = idx1;
			this->idx2 = idx2;
			this->tri_type = tri_type;
		}
	};
}
#endif
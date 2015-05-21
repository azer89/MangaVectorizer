
/*
 * Type of triangles
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 * 
 */

#ifndef __Triangle_Type__
#define __Triangle_Type__

namespace CVSystem
{
	typedef enum TriangleType
	{
		TRIANGLE_NOT_DEFINED = 0,	// Unused triangle (you shouldn't assign this value)
		TRIANGLE_BLACK	     = 1,	// black region
		TRIANGLE_WHITE	     = 2,	// white region
		TRIANGLE_SCREENTONE  = 3,	// Screentone region
	};
}
#endif
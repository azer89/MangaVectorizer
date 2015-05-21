
/**
 * Struct for triangle data
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __MYTRIANGLE_H__
#define __MYTRIANGLE_H__

#include "MyPoint.h"
#include "TriangleType.h"

namespace CVSystem
{
	struct MyTriangle
	{
	public:

		// Todo : change to p0, p1, and p3
		MyPoint p1;
		MyPoint p2;
		MyPoint p3;

		// type (black, white, and screentone)
		TriangleType tri_type;

		// constructor
		MyTriangle(MyPoint p1, MyPoint p2, MyPoint p3, TriangleType tri_type)
		{
			this->p1 = p1;
			this->p2 = p2;
			this->p3 = p3;

			this->tri_type = tri_type;
		}

		// scaling
		MyTriangle Resize(double val)
		{
			MyTriangle newT(this->p1.Resize(val), this->p2.Resize(val), this->p3.Resize(val), this->tri_type);
			return newT;
		}

		// center point of a triangle
		MyPoint GetCenter()
		{
			return MyPoint((p1.x + p2.x + p3.x) / 3.0, (p1.y + p2.y + p3.y) / 3.0);
		}
	};
}
#endif
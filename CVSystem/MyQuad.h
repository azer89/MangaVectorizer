
/**
 * Struct for quad data
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __MYQUAD_H__
#define __MYQUAD_H__

#include "MyPoint.h"

namespace CVSystem
{
	// Representation of quad with four vertices
	struct MyQuad
	{
	public:
		// start point
		MyPoint p0;

		// first control point
		MyPoint p1;

		// second control point
		MyPoint p2;
		
		// end point
		MyPoint p3;

		// constructor
		MyQuad(MyPoint p0, MyPoint p1, MyPoint p2, MyPoint p3)
		{
			this->p0 = p0;
			this->p1 = p1;
			this->p2 = p2;
			this->p3 = p3;
		}


		// scaling
		MyQuad Resize(double val)
		{
			MyQuad newQ(this->p0.Resize(val), this->p1.Resize(val), this->p2.Resize(val), this->p3.Resize(val));
			return newQ;
		}
	};
}
#endif
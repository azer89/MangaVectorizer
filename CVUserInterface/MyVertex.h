
/**
 * Struct representation for vertex
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __VERTEX_H__
#define __VERTEX_H__

#include <iostream>
#include "MyPoint.h"

struct MyVertex
{
public:
	// coordinate
	CVSystem::MyPoint xyCoor;

	// implicit bezier curve
	double k;	
	double l;
	double m;

	// Constructor without parameters
	MyVertex()
	{
		this->xyCoor.x = -1;
		this->xyCoor.y = -1;

		this->k = -1;
		this->l = -1;
		this->m = -1;
	}

	// Constructor with parameters
	MyVertex(float x, float y, double k, double l, double m)
	{
		this->xyCoor.x = x;
		this->xyCoor.y = y;

		this->k = k;
		this->l = l;
		this->m = m;
	}
};

#endif
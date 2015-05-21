
/**
 *
 * Draw curve for fun
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#ifndef __Color_Curve_Renderer__
#define __Color_Curve_Renderer__

#include <vector>
#include "MyPoint.h"
#include "MyIndexedBezierCurves.h"

// believe me, this class is not important
class ColorCurveRenderer
{
public:
	// Constructor
	ColorCurveRenderer();

	// Destructor
	~ColorCurveRenderer();

	// paint
	void paintGL();

	// set vertex list and bezier curve
	void SetData(std::vector<CVSystem::MyPoint>				  vertexList, 
				 std::vector<CVSystem::MyIndexedBezierCurves> indexedBezierCurves);

private:
	std::vector<CVSystem::MyPoint>				 _vertexList;			// storing each vertex position
	std::vector<CVSystem::MyIndexedBezierCurves> _indexedBezierCurves;	// original bezier curves
};

#endif
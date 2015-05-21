
/**
 *
 * Extract all vertices (points) and edges (curves) along a connected component
 * boundary.
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#ifndef __Arrangement_Extractor__
#define __Arrangement_Extractor__

#include <vector>
#include "MyPoint.h"

// Extract boundary
template<class Arrangement>
std::vector<CVSystem::MyPoint> ExtractCCB(typename Arrangement::Ccb_halfedge_const_circulator circ)
{
	// borders
	std::vector<CVSystem::MyPoint> boundary;

	typename Arrangement::Ccb_halfedge_const_circulator  curr = circ;
	typename Arrangement::Halfedge_const_handle          he;
	do
	{
		he = curr;

		double x = CGAL::to_double(he->target()->point().x());
		double y = CGAL::to_double(he->target()->point().y());
		boundary.push_back(CVSystem::MyPoint(x, y));

		++curr;
	} while (curr != circ);

	return boundary;
}


/**
 * Extract the boundary description of an arrangement face
 */
template<class Arrangement>
std::vector<std::vector<CVSystem::MyPoint>> ExtractFace (typename Arrangement::Face_const_handle f)
{
	std::vector<std::vector<CVSystem::MyPoint>> boundaries;

	//// Outer boundary.
	if (f->is_unbounded())
	{ std::cout << "We do not support unbounded face\n"; }
	else
	{
		std::vector<CVSystem::MyPoint> boundary = ExtractCCB<Arrangement> (f->outer_ccb());
		boundaries.push_back(boundary);
	}
	typename Arrangement::Hole_const_iterator  hole;
	int index = 1;

	//// Hole
	for (hole = f->holes_begin(); hole != f->holes_end(); ++hole, ++index)
	{		
		std::vector<CVSystem::MyPoint> boundary = ExtractCCB<Arrangement> (*hole);
		boundaries.push_back(boundary);
	}

	return boundaries;
}

#endif

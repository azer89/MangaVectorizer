
/**
 * Testing if a point is inside triangle mesh or not using AABB data structure
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#ifndef __Point_Test__
#define __Point_Test__

#include "MyPoint.h"
#include "MyTriangle.h"
#include "MyIndexedTriangle.h"

#include "CGALTools.h"


namespace CVSystem
{
	class PointInTrianglesTest
	{
	private:
		// CGAL Typedefs
		typedef CGAL::Simple_cartesian<double>							PTest_K;
		typedef PTest_K::Line_3											PTest_Line;
		typedef PTest_K::Point_3										PTest_Point;
		typedef PTest_K::Triangle_3										PTest_Triangle;
		typedef std::list<PTest_Triangle>::iterator						PTest_Iterator;
		typedef CGAL::AABB_triangle_primitive<PTest_K, PTest_Iterator>	PTest_Primitive;
		typedef CGAL::AABB_traits<PTest_K, PTest_Primitive>				PTest_AABB_triangle_traits;
		typedef CGAL::AABB_tree<PTest_AABB_triangle_traits>				PTest_Tree;

	public:
		// Constructor
		PointInTrianglesTest();

		// Destructor
		~PointInTrianglesTest();

		// Triangle mesh used for point test
		void PushTriangles(std::vector<CVSystem::MyTriangle> cv_triangles);

		// triangle mesh used for point test
		void PushTriangles(std::vector<CVSystem::MyPoint> vertexList, std::vector<CVSystem::MyIndexedTriangle> triangles);
		
		// Get distance of a point to the closest triangle
		double GetDistance(CVSystem::MyPoint pt);

		// Is a point inside of triangle mesh
		bool IsInside2(CVSystem::MyPoint pt);

		// Is a point inside of triangle mesh
		bool IsInside(CVSystem::MyPoint pt);

		// Is a point inside of triangle mesh
		bool IsInside(CD_Cdt::Face_handle fh);

	private:
		// Triangle mesh
		std::list<PTest_Triangle> ptest_triangles;

		// AABB tree
		PTest_Tree* ptest_tree;
	};
}

#endif


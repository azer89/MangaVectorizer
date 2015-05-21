
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "PointInTrianglesTest.h"

CVSystem::PointInTrianglesTest::PointInTrianglesTest()
{
}

CVSystem::PointInTrianglesTest::~PointInTrianglesTest()
{
}

void CVSystem::PointInTrianglesTest::PushTriangles(std::vector<CVSystem::MyPoint> vertexList, std::vector<CVSystem::MyIndexedTriangle> triangles)
{
	using namespace CVSystem;
	ptest_triangles.clear();
	for(size_t a = 0; a < triangles.size(); a++)
	{
		MyPoint cvPt0 = vertexList[triangles[a].idx0];
		MyPoint cvPt1 = vertexList[triangles[a].idx1];
		MyPoint cvPt2 = vertexList[triangles[a].idx2];

		PTest_Point p0(cvPt0.x, cvPt0.y, 0);
		PTest_Point p1(cvPt1.x, cvPt1.y, 0);
		PTest_Point p2(cvPt2.x, cvPt2.y, 0);

		ptest_triangles.push_back(PTest_Triangle(p0, p1, p2));
	}

	this->ptest_tree = new PTest_Tree(ptest_triangles.begin(), ptest_triangles.end());
	this->ptest_tree->accelerate_distance_queries();

	std::cout << "Point Test, num of triangles: " << triangles.size()<< "\n";
}

void CVSystem::PointInTrianglesTest::PushTriangles(std::vector<CVSystem::MyTriangle> cv_triangles)
{
	ptest_triangles.clear();
	for(size_t a = 0; a < cv_triangles.size(); a++)
	{
		PTest_Point p0(cv_triangles[a].p1.x, cv_triangles[a].p1.y, 0);
		PTest_Point p1(cv_triangles[a].p2.x, cv_triangles[a].p2.y, 0);
		PTest_Point p2(cv_triangles[a].p3.x, cv_triangles[a].p3.y, 0);

		ptest_triangles.push_back(PTest_Triangle(p0, p1, p2));
	}
	//PTest_Tree tree(cv_triangles.begin(),cv_triangles.end());
	this->ptest_tree = new PTest_Tree(ptest_triangles.begin(), ptest_triangles.end());
	this->ptest_tree->accelerate_distance_queries();

	std::cout << "Point Test, num of triangles: " << cv_triangles.size()<< "\n";
}

double CVSystem::PointInTrianglesTest::GetDistance(CVSystem::MyPoint pt)
{
	PTest_Point point_query(pt.x, pt.y, 0.0);
	return (double)this->ptest_tree->squared_distance(point_query);
}

bool CVSystem::PointInTrianglesTest::IsInside2(CVSystem::MyPoint pt)
{
	PTest_Point point_query(pt.x, pt.y, 0.0);
	//Point_intersection intersectPt = this->ptest_tree->any_intersection(point_query);
	//if(intersectPt) {return true;}
	return false;
}

bool CVSystem::PointInTrianglesTest::IsInside(CVSystem::MyPoint pt)
{
	PTest_Point point_query(pt.x, pt.y, 0.0);
	double sqd = (double)this->ptest_tree->squared_distance(point_query);
	if(sqd > -M_EPS && sqd < M_EPS) { return true; }
	return false;
}

bool CVSystem::PointInTrianglesTest::IsInside(CD_Cdt::Face_handle fh)
{
	double x = (fh->vertex(0)->point().x() + fh->vertex(1)->point().x() + fh->vertex(2)->point().x()) / 3;
	double y = (fh->vertex(0)->point().y() + fh->vertex(1)->point().y() + fh->vertex(2)->point().y()) / 3;
	return IsInside(MyPoint(x, y));
}

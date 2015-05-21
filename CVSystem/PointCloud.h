
/**
 * KD-Tree data
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#ifndef __Point_Cloud__
#define __Point_Cloud__



template <typename T>
struct PointCloud
{
	struct Point 
	{ 
		//// coordinate in 2D
		T x;
		T y; 

		//// Something 1
		int info1;

		//// Something 2
		int info2;
	};
	
	std::vector<Point>  pts;

	inline void PushPoint(T x, T y) const { pts.push_back(Point(x, y)); }

	// for nanoflann internal use
	inline size_t kdtree_get_point_count() const { return pts.size(); }

	// for nanoflann internal use
	inline T kdtree_distance(const T *p1, const size_t idx_p2, size_t size) const
	{
		const T d0 = p1[0] - pts[idx_p2].x;
		const T d1 = p1[1] - pts[idx_p2].y;
		return sqrt(d0 * d0 + d1 * d1);
	}

	// for nanoflann internal use
	inline T kdtree_get_pt(const size_t idx, int dim) const
	{
		if (dim == 0) { return pts[idx].x; }
		else { return pts[idx].y; }
	}

	// for nanoflann internal use
	template <class BBOX>
	bool kdtree_get_bbox(BBOX &bb) const { return false; }
};

#endif
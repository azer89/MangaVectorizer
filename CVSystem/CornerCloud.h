
/**
 * Fast query of corner points by using KD-Tree
 * Corner points are defined as a point with two direction vectors
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#ifndef __Corner_Cloud__
#define __Corner_Cloud__


template <typename T>
struct CornerCloud
{
	// corner data
	struct CornerData 
	{ 
		T x;
		T y;

		// prev point
		T prevX;
		T prevY;

		// next point
		T nextX;
		T nextY;
	};

	// corner list
	std::vector<CornerData> corners;

	// for nanoflann internal use
	inline size_t kdtree_get_point_count() const { return corners.size(); }

	// for nanoflann internal use
	inline T kdtree_distance(const T *p1, const size_t idx_p2, size_t size) const
	{
		// simple euclidean distance
		const T d0 = p1[0] - corners[idx_p2].x;
		const T d1 = p1[1] - corners[idx_p2].y;
		return sqrt(d0 * d0 + d1 * d1);
	}

	// for nanoflann internal use
	inline T kdtree_get_pt(const size_t idx, int dim) const
	{
		// get x or y values
		if (dim == 0) return corners[idx].x;
		else return corners[idx].y;
	}

	// for nanoflann internal use
	template <class BBOX>
	bool kdtree_get_bbox(BBOX &bb) const { return false; }
};

#endif
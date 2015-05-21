
#pragma once

#include "targetver.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <iostream>
#include <vector>
#include <fstream>
#include <list>

#include <string>

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Spatial_sort_traits_adapter_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Segment_2.h>

#include <CGAL/Delaunay_mesher_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Delaunay_mesh_size_criteria_2.h>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_triangle_primitive.h>

// Eigen for matrix operation
#include <Eigen/Dense>
#include <Eigen/Core>
#include <Eigen/SVD>

// KD Tree (somehow this should be put in stdafx.h)
#include "NanoFLANN.h"

// Constants
#ifndef M_PI
#define M_PI		3.14159265358979323846264338327950288
#endif

#ifndef M_EPS
#define M_EPS		1e-8
#endif

#ifndef DIVIDE_EPS
#define DIVIDE_EPS	1e-16
#endif

#ifndef SQRT2
#define SQRT2 1.41421356237309504880
#endif


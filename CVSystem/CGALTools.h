
/**
 * This file does some important declaration of CGAL types
 * where there are bunch of typedefs
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#ifndef __CGAL_Tools__
#define __CGAL_Tools__

#include "Delaunay_mesh_face_base_info_2.h"

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


namespace CVSystem
{
	//// Custom struct
	struct FaceInfo2
	{
		FaceInfo2() { }
		int nesting_level;
		bool in_domain() { return nesting_level%2 == 1; }
	};

	typedef CGAL::Exact_predicates_inexact_constructions_kernel			 K;
	typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>			 Vb;
	typedef CGAL::Polygon_2<K>											 Polygon_2;
	typedef CGAL::Exact_predicates_tag									 Itag;

	//// Conformed Delaunay
	typedef CGAL::Delaunay_mesh_face_base_info_2<K>						CD_Fb;
	typedef CGAL::Triangulation_data_structure_2<Vb, CD_Fb>			    CD_Tds;
	typedef CGAL::Constrained_Delaunay_triangulation_2<K, CD_Tds, Itag> CD_Cdt;	
	typedef CD_Cdt::Vertex_handle										CD_VHandle;
	typedef CD_Cdt::Point												CD_Point;

	//// Mesher
	typedef CGAL::Delaunay_mesh_size_criteria_2<CD_Cdt>	 CD_Criteria;	// Triangulation's criteria

	//// Polygonal Domain Triangulation 
	typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo2,K>      PD_Fbb;	
	typedef CGAL::Constrained_triangulation_face_base_2<K,PD_Fbb>       PD_Fb;
	typedef CGAL::Triangulation_data_structure_2<Vb,PD_Fb>              PD_Tds;
	
	typedef CGAL::Constrained_Delaunay_triangulation_2<K, PD_Tds, Itag> PD_Cdt;
	typedef PD_Cdt::Point                                               PD_Point;

	// Insert a polygon to cdt
	void insert_polygon(PD_Cdt& cdt,const Polygon_2& polygon);		

	// Mark domains
	void mark_domains(PD_Cdt& cdt);

	// Mark domains
	void mark_domains(PD_Cdt& ct, PD_Cdt::Face_handle start, int index, std::list<PD_Cdt::Edge>& border );

	// Check two handles identical
	bool AreHandlesIdentical(CD_VHandle vh0, CD_VHandle vh1);
}

#endif
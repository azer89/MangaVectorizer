
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "CGALTools.h"

void CVSystem::insert_polygon(PD_Cdt& cdt, const Polygon_2& polygon)
{
	if ( polygon.is_empty() ) return;
	PD_Cdt::Vertex_handle v_prev=cdt.insert(*CGAL::cpp11::prev(polygon.vertices_end()));
	for (Polygon_2::Vertex_iterator vit = polygon.vertices_begin(); vit != polygon.vertices_end();++vit)
	{
		PD_Cdt::Vertex_handle vh=cdt.insert(*vit);
		//if(vh != v_prev)
		cdt.insert_constraint(vh,v_prev);
		v_prev=vh;
	}  
}

void CVSystem::mark_domains(PD_Cdt& ct, PD_Cdt::Face_handle start, int index, std::list<PD_Cdt::Edge>& border )
{
	if(start->info().nesting_level != -1) return;

	std::list<PD_Cdt::Face_handle> queue;
	queue.push_back(start);
	while(! queue.empty())
	{
		PD_Cdt::Face_handle fh = queue.front();
		queue.pop_front();
		if(fh->info().nesting_level == -1)
		{
			fh->info().nesting_level = index;
			for(int i = 0; i < 3; i++)
			{
				PD_Cdt::Edge e(fh,i);
				PD_Cdt::Face_handle n = fh->neighbor(i);
				if(n->info().nesting_level == -1)
				{
					if(ct.is_constrained(e)) border.push_back(e);
					else queue.push_back(n);
				}
			}
		}
	}
}

void CVSystem::mark_domains(PD_Cdt& cdt)
{
	for(PD_Cdt::All_faces_iterator it = cdt.all_faces_begin(); it != cdt.all_faces_end(); ++it)
	{
		it->info().nesting_level = -1;
	}
	std::list<PD_Cdt::Edge> border;
	mark_domains(cdt, cdt.infinite_face(), 0, border);
	while(! border.empty())
	{
		PD_Cdt::Edge e = border.front();
		border.pop_front();
		PD_Cdt::Face_handle n = e.first->neighbor(e.second);
		if(n->info().nesting_level == -1){
			mark_domains(cdt, n, e.first->info().nesting_level+1, border);
		}
	}
}

bool CVSystem::AreHandlesIdentical(CD_VHandle vh0, CD_VHandle vh1)
{
	double x0 = vh0->point().x();
	double y0 = vh0->point().y();

	double x1 = vh1->point().x();
	double y1 = vh1->point().y();

	if(abs(x0 - x1) < M_EPS && abs(y0 - y1) < M_EPS)
	{ return true;}

	return false;
}


/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "OBJIO.h"

#include "UtilityFunctions.h"

#include <iostream>
#include <fstream>
#include <iomanip>


void CVSystem::OBJIO::WriteToOBJ(std::string filename, CVSystem::CD_Cdt cdt)
{
	using namespace std;

	ofstream f;
	f.open (filename);
	
	// warning, not sorted
	for (CD_Cdt::Finite_vertices_iterator vit = cdt.finite_vertices_begin(); vit != cdt.finite_vertices_end(); ++vit) 
	{ 
		if(vit->info() >= 0)
		{
			f << "v " << " " <<
				 std::setprecision(20) << vit->point().x() << " " << 
				 std::setprecision(20) << vit->point().y() << " " <<
				 0 << "\n";
		}
	}

	for(CD_Cdt::Finite_faces_iterator fit = cdt.finite_faces_begin(); fit != cdt.finite_faces_end(); ++fit) 
	{
		if(fit->is_in_domain())
		{
			CD_VHandle vh0 = fit->vertex(0);
			CD_VHandle vh1 = fit->vertex(1);
			CD_VHandle vh2 = fit->vertex(2);

			f << "f " << " " <<
				 (vh0->info() + 1) << " " << 
				 (vh1->info() + 1) << " " << 
				 (vh2->info() + 1) << "\n";
		}
	}

	f.close();
}

void CVSystem::OBJIO::ReadFromMYOBJ(std::string							meshFilename,
									int&								img_width,
									int&								img_height,
									std::vector<int>&					offsets,
									std::vector<MyPoint>&				vertexList,				// The actual vertices
									std::vector<MyIndexedTriangle>&     indexedTriangles,		// All triangles
									std::vector<MyIndexedTriangle>&     borderSCTriangles,		// Additional screentone triangles
									std::vector<MyIndexedTriangle>&     borderWTriangles,		// Additional white triangles
									std::vector<MyIndexedTriangle>&     scTriangles,			// Screentone triangles
									std::vector<MyIndexedTriangle>&     wTriangles,				// White triangles
									std::vector<MyIndexedTriangle>&     bTriangles,				// Black triangles
									std::vector<MyIndexedBezierCurves>& indexedBezierCurves,	// Bezier curves (start and end points only)
									std::vector<MyQuad>&		        lsQuads)	
{
	using namespace CVSystem;

	vertexList.clear();
	indexedTriangles.clear();
	borderSCTriangles.clear();
	borderWTriangles.clear();
	scTriangles.clear();
	wTriangles.clear();
	bTriangles.clear();
	indexedBezierCurves.clear();
	lsQuads.clear();

	std::ifstream myfile (meshFilename);
	while(!myfile.eof())
	{
		std::string line;
		std::getline (myfile,line);
		
		if(line.size() == 0) {continue;}
		
		std::vector<std::string> arrayStr = UtilityFunctions::split(line, ' ');
		
		// vertex list
		if(StartWith("v2d", line) && arrayStr.size() == 3)
		{
			double x = std::stod(arrayStr[1]);
			double y = std::stod(arrayStr[2]);
			vertexList.push_back(MyPoint(x, y));
		}

		// all triangles
		else if(StartWith("ft", line) && arrayStr.size() == 5)
		{
			MyIndexedTriangle tr( std::stoi(arrayStr[1]),
				                  std::stoi(arrayStr[2]),
								  std::stoi(arrayStr[3]),
								  (TriangleType)std::stoi(arrayStr[4]));

			indexedTriangles.push_back(tr);

			if(tr.tri_type == TRIANGLE_SCREENTONE) { scTriangles.push_back(tr); }
			else if(tr.tri_type == TRIANGLE_WHITE) { wTriangles.push_back(tr); }
			else if(tr.tri_type == TRIANGLE_BLACK) { bTriangles.push_back(tr); }
		}

		// border screentone triangle
		else if(StartWith("fbsc", line) && arrayStr.size() == 4)
		{
			MyIndexedTriangle tr( std::stoi(arrayStr[1]),
								std::stoi(arrayStr[2]),
								std::stoi(arrayStr[3]),
								TRIANGLE_SCREENTONE);
			borderSCTriangles.push_back(tr);
		}

		else if(StartWith("fbw", line) && arrayStr.size() == 4)
		{
			MyIndexedTriangle tr( std::stoi(arrayStr[1]),
				std::stoi(arrayStr[2]),
				std::stoi(arrayStr[3]),
				TRIANGLE_WHITE);
			borderWTriangles.push_back(tr);
		}

		else if(StartWith("ibz", line))
		{
			MyIndexedBezierCurves bzc;
			for(size_t a = 1; a < arrayStr.size(); a++) { bzc.indices.push_back( std::stoi(arrayStr[a])  ); }

			//bzc.p0_points.resize(bzc.indices.size());
			//bzc.p1_points.resize(bzc.indices.size());
			//bzc.p2_points.resize(bzc.indices.size());

			indexedBezierCurves.push_back(bzc);
		}

		else if(StartWith("q", line) && arrayStr.size() == 9)
		{
			MyQuad qd(MyPoint(std::stod(arrayStr[1]), std::stod(arrayStr[2])), 
					  MyPoint(std::stod(arrayStr[3]), std::stod(arrayStr[4])), 
					  MyPoint(std::stod(arrayStr[5]), std::stod(arrayStr[6])), 
					  MyPoint(std::stod(arrayStr[7]), std::stod(arrayStr[8])));

			lsQuads.push_back(qd);
			//lsQuads.push_back(qd);
		}

		else if(StartWith("iw", line) && arrayStr.size() == 2)
		{
			img_width = std::stoi(arrayStr[1]);
		}

		else if(StartWith("ih", line) && arrayStr.size() == 2)
		{
			img_height = std::stoi(arrayStr[1]);
		}

		else if(StartWith("of", line) && arrayStr.size() == 9)
		{
			offsets[0] = std::stoi(arrayStr[1]);
			offsets[1] = std::stoi(arrayStr[2]);
			offsets[2] = std::stoi(arrayStr[3]);
			offsets[3] = std::stoi(arrayStr[4]);
			offsets[4] = std::stoi(arrayStr[5]);
			offsets[5] = std::stoi(arrayStr[6]);
			offsets[6] = std::stoi(arrayStr[7]);
			offsets[7] = std::stoi(arrayStr[8]);
			//part_offset = std::stoi(arrayStr[1]);
		}
		
		//if (line[0] == 'v')
		//{
		//	myfile >> v >> valuesX[n]>> valuesY[n]>> valuesZ[n];
		//	cout << valuesX[n] << "\t" << valuesY[n] << "\t" << valuesZ[n] << endl;
		//	n++;
		//}
	}

	myfile.close();
}

void CVSystem::OBJIO::WriteMyToOBJ(std::string		meshFilename, 
								   int				img_width,
								   int				img_height,
								   std::vector<int>							  offsets,
								   std::vector<CVSystem::MyPoint>				  vertexList, 
								   std::vector<CVSystem::MyIndexedBezierCurves>	  indexedBezierCurves, 
								   std::vector<MyIndexedTriangle>				  borderSCTriangles,
								   std::vector<MyIndexedTriangle>				  borderWTriangles,
								   std::vector<CVSystem::MyQuad>				  bzQuads,
								   std::vector<CVSystem::MyIndexedTriangle>		  indexedTriangles)
{
	using namespace std;
	using namespace CVSystem;

	ofstream f;
	f.open (meshFilename);

	f << "iw " << img_width << "\n";
	f << "ih " << img_height << "\n";

	f << "of " << offsets[0] << " "
			   << offsets[1] << " "
			   << offsets[2] << " "
			   << offsets[3] << " "
			   << offsets[4] << " "
			   << offsets[5] << " "
			   << offsets[6] << " "
			   << offsets[7] << "\n";

	// vertex list
	for(size_t a = 0; a < vertexList.size(); a++)
	{
		f << "v2d" << " " <<
			 std::setprecision(20) << vertexList[a].x << " " << 
			 std::setprecision(20) << vertexList[a].y << "\n";
	}

	// all triangles
	for(size_t a = 0; a < indexedTriangles.size(); a++)
	{
		f << "ft" << " " <<
			 indexedTriangles[a].idx0 << " " << 
			 indexedTriangles[a].idx1 << " " << 
			 indexedTriangles[a].idx2 << " " <<
			 (unsigned)indexedTriangles[a].tri_type << "\n";
	}

	// border screentone triangle
	for(size_t a = 0; a < borderSCTriangles.size(); a++)
	{
		f << "fbsc" << " " <<
			 borderSCTriangles[a].idx0 << " " << 
			 borderSCTriangles[a].idx1 << " " << 
			 borderSCTriangles[a].idx2 << "\n";
	}

	// border white triangle
	for(size_t a = 0; a < borderWTriangles.size(); a++)
	{
		f << "fbw" << " " <<
			 borderWTriangles[a].idx0 << " " << 
			 borderWTriangles[a].idx1 << " " << 
			 borderWTriangles[a].idx2 << "\n";
	}

	// indexed bezier curves, only store start and end 
	for(size_t a = 0; a < indexedBezierCurves.size(); a++)
	{
		MyIndexedBezierCurves bzc = indexedBezierCurves[a];
		f << "ibz" << " ";
		for(size_t b = 0; b < bzc.indices.size(); b++)
		{
			f << bzc.indices[b];
			if(b < bzc.indices.size() -1) {f << " ";}
			else {f << "\n";}
		}
	}

	// original ls bezier (not indexed)
	for(size_t a = 0; a < bzQuads.size(); a++)
	{
		f << "q" << " " <<

			bzQuads[a].p0.x << " " << 
			bzQuads[a].p0.y << " " <<

			bzQuads[a].p1.x << " " <<
			bzQuads[a].p1.y << " " << 
			
			bzQuads[a].p2.x << " " <<
			bzQuads[a].p2.y << " " <<
			
			bzQuads[a].p3.x << " " <<
			bzQuads[a].p3.y;

		if(a <  bzQuads.size() - 1) {f << "\n";}
	}

	f.close();
}


/**
 * Read, write custom mesh .myobj file
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __OBJ_IO__
#define __OBJ_IO__

#include <iostream>
#include <vector>
#include <fstream>

#include "CGALTools.h"
#include "MyPoint.h"
#include "MyIndexedBezierCurves.h"
#include "MyIndexedTriangle.h"
#include "MyQuad.h"

namespace CVSystem
{
	class OBJIO
	{
	public:
		//// warning, the vertex indices isn't sorted
		static void WriteToOBJ(std::string filename, CVSystem::CD_Cdt cdt);

		//// write .MYOBJ file
		static void WriteMyToOBJ(std::string filename, 
								 int		 img_width,
								 int		 img_height,
								 std::vector<int>							  offsets,
								 std::vector<CVSystem::MyPoint>				  vertexList, 
							     std::vector<CVSystem::MyIndexedBezierCurves> indexedBezierCurves,
							     std::vector<MyIndexedTriangle>				  borderSCTriangles,
							     std::vector<MyIndexedTriangle>				  borderWTriangles,
							     std::vector<MyQuad>						  bzQuads,
							     std::vector<CVSystem::MyIndexedTriangle>	  indexedTriangles);
		//// read .MYOBJ file
		static void ReadFromMYOBJ(std::string						  filename,
								  int&								  img_width,
								  int&								  img_height,
								  std::vector<int>&					  offsets,
								  std::vector<MyPoint>&			      vertexList,			// The actual vertices
								  std::vector<MyIndexedTriangle>&     indexedTriangles,		// All triangles
								  std::vector<MyIndexedTriangle>&     borderSCTriangles,	// Additional screentone triangles
								  std::vector<MyIndexedTriangle>&     borderWTriangles,		// Additional white triangles
								  std::vector<MyIndexedTriangle>&     scTriangles,			// Screentone triangles
								  std::vector<MyIndexedTriangle>&     wTriangles,			// White triangles
								  std::vector<MyIndexedTriangle>&     bTriangles,			// Black triangles
								  std::vector<MyIndexedBezierCurves>& indexedBezierCurves,	// Bezier curves (start and end points only)
								  std::vector<MyQuad>&		          lsQuads);	

	private:
		// string startwith
		static bool StartWith(std::string prefix, std::string argument)
		{
			if(argument.substr(0, prefix.size()) == prefix) 
				{ return true; }
			return false;
		}
	};
}

#endif


/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "LinesSorter.h"

std::vector<CVSystem::MyIndexedBezierCurves> CVSystem::LinesSorter::BuildBezierCurves2(std::vector<MyIndexedLine> inputLines, std::vector<MyPoint> vertexList)
{
	using namespace nanoflann;

	std::vector<MyIndexedBezierCurves> completeCurves;   // closed

	LineCloud<float> lineCloud;
	lineCloud.lines.resize(inputLines.size());

	//std::vector<bool> flagList(inputLines.size());
	std::vector<bool> vFlag(vertexList.size());
	for(size_t a = 0; a < vertexList.size(); a++)
		{ vFlag[a] = false; }

	for(size_t a = 0; a < inputLines.size(); a++)
	{
		//flagList[a] = false;

		MyPoint ptA = vertexList[inputLines[a].index0]; MyPoint ptB = vertexList[inputLines[a].index1];
		lineCloud.lines[a].x = ptA.x; 
		lineCloud.lines[a].y = ptA.y;
		lineCloud.lines[a].index0 = inputLines[a].index0; 
		lineCloud.lines[a].index1 = inputLines[a].index1;
	}

	typedef KDTreeSingleIndexAdaptor< L2_Simple_Adaptor<float, LineCloud<float> >, LineCloud<float>, 2 /*dim*/> my_kd_tree_t;
	my_kd_tree_t   index(2 /*dim*/, lineCloud, KDTreeSingleIndexAdaptorParams(10000 /* max leaf */) );
	index.buildIndex();

	for(size_t a = 0; a < inputLines.size(); a++)
	{
		//if(flagList[a]) { continue; }
		if(vFlag[inputLines[a].index0] && vFlag[inputLines[a].index1]) { continue; }

		//flagList[a] = true;
		vFlag[inputLines[a].index0] = true;
		vFlag[inputLines[a].index1] = true;

		MyIndexedBezierCurves newPoly;
		newPoly.indices.push_back(inputLines[a].index0);
		newPoly.indices.push_back(inputLines[a].index1);

		while(/*!newPoly.IsClosed(vertexList)*/ true)
		{
			MyPoint endPoint = vertexList[newPoly.indices[newPoly.indices.size() - 1]];

			float query_pt[2] = { endPoint.x, endPoint.y};
			std::vector<size_t> ret_index(1);
			std::vector<float> out_dist_sqr(1);
			index.knnSearch(&query_pt[0], 1, &ret_index[0], &out_dist_sqr[0]);
			double dist = out_dist_sqr[0];

			if(dist > -M_EPS && dist < M_EPS)
			{
				int retIdx = ret_index[0];
				bool shouldStop = false;
				

				if(vFlag[lineCloud.lines[retIdx].index1])
					{ /* STOP */	shouldStop = true; }	

				//flagList[retIdx] = true;
				vFlag[lineCloud.lines[retIdx].index1] = true;
				newPoly.indices.push_back(lineCloud.lines[retIdx].index1);

				if(shouldStop){break;}
			}
			else
			{ std::cout << "error\n"; }
		}
		completeCurves.push_back(newPoly);
	}

	return completeCurves;
}

std::vector<CVSystem::MyIndexedBezierCurves> CVSystem::LinesSorter::BuildBezierCurves1(std::vector<MyIndexedLine> inputLines, std::vector<MyPoint> vertexList)
{
	using namespace nanoflann;

	std::vector<MyIndexedBezierCurves> completeCurves;   // closed

	LineCloud<float> lineCloud;
	lineCloud.lines.resize(inputLines.size());

	std::vector<bool> flagList(inputLines.size());

	for(size_t a = 0; a < inputLines.size(); a++)
	{
		flagList[a] = false;
		
		MyPoint ptA = vertexList[inputLines[a].index0]; MyPoint ptB = vertexList[inputLines[a].index1];
		lineCloud.lines[a].x = ptA.x; lineCloud.lines[a].y = ptA.y;
		lineCloud.lines[a].index0 = inputLines[a].index0; lineCloud.lines[a].index1 = inputLines[a].index1;
	}

	typedef KDTreeSingleIndexAdaptor< L2_Simple_Adaptor<float, LineCloud<float> >, LineCloud<float>, 2 /*dim*/> my_kd_tree_t;
	my_kd_tree_t   index(2 /*dim*/, lineCloud, KDTreeSingleIndexAdaptorParams(10000 /* max leaf */) );
	index.buildIndex();

	for(size_t a = 0; a < inputLines.size(); a++)
	{
		if(flagList[a]) { continue; }

		flagList[a] = true;
		MyIndexedBezierCurves newPoly;
		newPoly.indices.push_back(inputLines[a].index0);
		newPoly.indices.push_back(inputLines[a].index1);

		while(!newPoly.IsClosed(vertexList))
		{
			MyPoint endPoint = vertexList[newPoly.indices[newPoly.indices.size() - 1]];

			float query_pt[2] = { endPoint.x, endPoint.y};
			std::vector<size_t> ret_index(1);
			std::vector<float> out_dist_sqr(1);
			index.knnSearch(&query_pt[0], 1, &ret_index[0], &out_dist_sqr[0]);
			double dist = out_dist_sqr[0];

			if(dist > -M_EPS && dist < M_EPS)
			{
				int retIdx = ret_index[0];
				flagList[retIdx] = true;
				newPoly.indices.push_back(lineCloud.lines[retIdx].index1);
			}
			else
			{ std::cout << "error\n"; }
		}
		completeCurves.push_back(newPoly);
	}

	return completeCurves;
}


/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "NonRigidPointMatch.h"
#include "SystemParams.h"
#include "PointMatch.h"
#include "Tools.h"
#include <math.h>

void NonRigidPointMatch::DoPointMatch(std::vector<CVSystem::MyPoint>& srcPoints, std::vector<CVSystem::MyPoint>& dstPoints, std::vector<CVSystem::MyLine>& correspondenceLines)
{
	using namespace CVSystem;

	// set parameters
	relax_graph_match_flag	= 0;
	rotate_invariant_flag	= 0;
	affine_LMS_flag			= 0;
	all_match_flag			= 0;

	E_Ave					= 5;	// 5
	T_Init					= 0.0;
	I_Max					= SystemParams::pe_num_iteration;
	init_label_outlier_flag	= 0;	// keep this zero

	MYPOINT* srcData = new MYPOINT[srcPoints.size()];
	MYPOINT* destData = new MYPOINT[dstPoints.size()];

	for(size_t a = 0; a < srcPoints.size(); a++)
		{ srcData[a].x = srcPoints[a].x; srcData[a].y = srcPoints[a].y; }

	for(size_t a = 0; a < dstPoints.size(); a++)
		{ destData[a].x = dstPoints[a].x; destData[a].y = dstPoints[a].y; }

	//PointMatch(srcData, srcPoints.size(), destData, dstPoints.size());
	PointMatchOutlier(srcData, srcPoints.size(), destData, dstPoints.size());

	for(size_t a = 0; a < srcPoints.size(); a++)
	{
		srcPoints[a].x = srcData[a].x;
		srcPoints[a].y = srcData[a].y;
	}

	//for(size_t a = 0; a < dstPoints.size(); a++)
	//{
	//	dstPoints[a].x = destData[a].x;
	//	dstPoints[a].y = destData[a].y;
	//}

	for(size_t a = 0; a < srcPoints.size(); a++)
	{
		MYPOINT cPoint = destData[srcData[a].nMatch];
		correspondenceLines.push_back(MyLine(srcData[a].x, srcData[a].y, cPoint.x, cPoint.y));
	}
	
	delete srcData;
	delete destData;
}



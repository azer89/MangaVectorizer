
/**
 * Non rigid registration to snap sribbles to line borders.
 *
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#ifndef __Non_Rigid_Point_Match__
#define __Non_Rigid_Point_Match__

#include "MyPoint.h"
#include "MyLine.h"

class NonRigidPointMatch
{
public:
	// non rigid registration proxy
	static void DoPointMatch(std::vector<CVSystem::MyPoint>& srcPoints, std::vector<CVSystem::MyPoint>& dstPoints, std::vector<CVSystem::MyLine>& correspondenceLines);
};

#endif


/**
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __KD_Tree_Declarations__
#define __KD_Tree_Declarations__

#include "NanoFLANN.h"
#include "LineCloud.h"
#include "PointCloud.h"
#include "CornerCloud.h"

using namespace nanoflann;

// Typedefs of several kd-tree types

// point
typedef KDTreeSingleIndexAdaptor< L2_Simple_Adaptor<float, PointCloud<float> >, 
							      PointCloud<float>, 
								  2 /*dim*/> 
								  PointKDTree;

// line
typedef KDTreeSingleIndexAdaptor< L2_Simple_Adaptor<float, LineCloud<float> >, 
								  LineCloud<float>, 
								  2 /*dim*/> 
								  LineKDTree;

// corner
typedef KDTreeSingleIndexAdaptor< L2_Simple_Adaptor<float, CornerCloud<float> >, 
								  CornerCloud<float>, 
								  2 /*dim*/> 
								  CornerKDTree;


#endif

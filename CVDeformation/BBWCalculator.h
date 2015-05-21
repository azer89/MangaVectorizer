
/**
 * This is a proxy class to LIBIGL with the sole purpose to 
 * calculate weights of bounded biharmonic
 *
 * Jacobson, A.; Baran, I.; Popović, J. & Sorkine, O. 
 * Bounded Biharmonic Weights for Real-Time Deformation 
 * ACM Transactions on Graphics (proceedings of ACM SIGGRAPH), 2011, 30, 78:1-78:8
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#ifndef __BBW_Calculator__
#define __BBW_Calculator__

#include <Eigen/Dense>
#include "MyPoint.h"
#include "MyIndexedTriangle.h"


 class BBWCalculator
{
public:
	// Calculate weights for every vertices
	static Eigen::MatrixXd GetWeights(std::vector<CVSystem::MyPoint> vertexList, std::vector<CVSystem::MyIndexedTriangle> indexedTriangles, std::vector<CVSystem::MyPoint> handles);

private:

	// These function below are for debugging...
	// You can delete these three functions

	// Finding max-min values of a matrix
	static void MaxMin(Eigen::MatrixXd V, double& xMin, double& xMax, double& yMin, double& yMax);

	// Interpolate a color using a factor d
	static void GetInterpolationColors(double d, int* b, int* g, int* r);

	// Display weight in an image for debugging
	static void DebugWeight(Eigen::MatrixXd V, Eigen::MatrixXd W, Eigen::MatrixXd C, Eigen::VectorXi P);
};

#endif
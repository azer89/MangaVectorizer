
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "BBWCalculator.h"

#include "UtilityFunctions.h"

Eigen::MatrixXd BBWCalculator::GetWeights(std::vector<CVSystem::MyPoint>		    vertexList, 
										  std::vector<CVSystem::MyIndexedTriangle> indexedTriangles, 
										  std::vector<CVSystem::MyPoint>			handles)
{
	std::cout << "vertexList.size() " <<	   vertexList.size() << "\n";
	std::cout << "indexedTriangles.size() " << indexedTriangles.size() << "\n";
	std::cout << "handles.size() " <<		   handles.size() << "\n";


	using namespace Eigen;
	using namespace igl;

	MatrixXd V(vertexList.size(), 3);		// #V by 3 list of mesh vertex positions
	MatrixXi F(indexedTriangles.size(), 3);	// #F by 3 list of triangle indices

	for(size_t a = 0; a < vertexList.size(); a++)
	{
		V.row(a)(0) = vertexList[a].x;
		V.row(a)(1) = vertexList[a].y;
		V.row(a)(2) = 0;
	}

	for(size_t a = 0; a < indexedTriangles.size(); a++)
	{
		F.row(a)(0) = indexedTriangles[a].idx0;
		F.row(a)(1) = indexedTriangles[a].idx1;
		F.row(a)(2) = indexedTriangles[a].idx2;
	}

	MatrixXd C(handles.size(), 3);		// List of control and joint (bone endpoint) positions
	VectorXi P(handles.size(), 1);		// List of point handles indexing C
	MatrixXi BE(0, 0);					// List of bone edges indexing C
	MatrixXi CE(0, 0);					// List of cage edges indexing *P*

	for(size_t a = 0; a < handles.size(); a++)
	{
		C.row(a)(0) = handles[a].x;
		C.row(a)(1) = handles[a].y;
		C.row(a)(2) = 0;

		P.row(a)(0) = a;
	}

	VectorXi b;		// // List of boundary indices (aka fixed value indices into VV)
	MatrixXd bc;	// List of boundary conditions of each weight function
	if(!boundary_conditions(V, F, C, P, BE, CE, b, bc)) { std::cout << "boundary condition failed\n";}

	// compute BBW, Default bbw data and flags
	BBWData bbw_data_mosek;
	bbw_data_mosek.qp_solver = QP_SOLVER_MOSEK;

	MatrixXd W;	// Weights matrix
	std::cout << "Calculating BBW, using QP_SOLVER_MOSEK\n";
	if(!bbw(V,F,b,bc,bbw_data_mosek,W)) 
	{ 
		std::cout << "QP_SOLVER_MOSEK failed, use QP_SOLVER_IGL_ACTIVE_SET\n";
		BBWData bbw_data_as;
		bbw_data_as.qp_solver = QP_SOLVER_IGL_ACTIVE_SET;
		bbw(V,F,b,bc,bbw_data_as,W);
	}

	normalize_row_sums(W, W);
	//DebugWeight(V, W, C, P);

	return W;
}

void BBWCalculator::MaxMin(Eigen::MatrixXd V, double& xMin, double& xMax, double& yMin, double& yMax)
{
	xMin = DBL_MAX;
	xMax = DBL_MIN;
	yMin = DBL_MAX;
	yMax = DBL_MIN;

	for(int a = 0; a < V.rows(); a++)
	{
		double x =  V(a, 0);
		double y =  V(a, 1);

		if(x > xMax) xMax = x;
		if(x < xMin) xMin = x;
		if(y > yMax) yMax = y;
		if(y < yMin) yMin = y;
	}

	std::cout << xMin << " " << xMax << " " << yMin << " " << yMax << "\n";
}

void BBWCalculator::GetInterpolationColors(double d, int* b, int* g, int* r)
{
	int blue = 0;
	int green = 0;
	int red = 0;

	if (d < 0.5)
	{
		blue   = (255 * d * 2.0) +  0   * (0.5 - d) * 2.0;
		green  = (255 * d * 2.0) +  0   * (0.5 - d) * 2.0;
		red    = (0   * d * 2.0) +  255 * (0.5 - d) * 2.0;
	}
	else
	{
		blue   = 255 * (d - 0.5) * 2.0 + 255 * (1.0 - d) * 2.0;
		green  = 0   * (d - 0.5) * 2.0 + 255 * (1.0 - d) * 2.0;  
		red    = 0   * (d - 0.5) * 2.0 + 0   * (1.0 - d) * 2.0;  
	}

	(*b) = blue;
	(*g) = green;
	(*r) = red;
}


void BBWCalculator::DebugWeight(Eigen::MatrixXd V, Eigen::MatrixXd W, Eigen::MatrixXd C, Eigen::VectorXi P)
{
	double xMin, xMax, yMin, yMax;
	MaxMin(V, xMin, xMax, yMin, yMax);

	for(int a = 0; a < W.cols(); a++)
	{
		cv::Mat drawImg = cv::Mat::zeros(yMax + 10, xMax + 10, CV_8UC3);

		for(int b = 0; b < V.rows(); b++)
		{
			double d = W(b, a);
			int x = V(b, 0);
			int y = V(b, 1);
			int _b, _g, _r;
			GetInterpolationColors(d, &_b, &_g, &_r);
			drawImg.at<cv::Vec3b>(y, x) = cv::Vec3b(_b, _g, _r);

			int hIdx = P(a, 0);
			x = C(hIdx, 0);
			y = C(hIdx, 1);

			cv::circle(drawImg, cv::Point(x, y), 3, cv::Scalar(0, 255, 0), -1);
		}

		std::stringstream sstr;
		sstr << "bbw_weight_" << a << ".png";
		cv::imwrite(sstr.str(), drawImg);
	}
}
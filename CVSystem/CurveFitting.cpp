
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "CurveFitting.h"

bool CVSystem::CurveFitting::PointstoBezier(std::vector<CVSystem::MyPoint> points, std::vector<CVSystem::MyPoint>& P)
{
	bool retVal = true;
	P = std::vector<CVSystem::MyPoint>(4);

	Eigen::MatrixXd M(4,4);
	M << -1,  3,  -3, 1,
		  3, -6,   3, 0,
		 -3,  3,   0, 0,
		  1,  0,   0, 0;

	Eigen::MatrixXd MInv = M.inverse();

	Eigen::MatrixXd U = GetU(points);
	Eigen::MatrixXd UT = U.transpose();

	Eigen::MatrixXd  X = GetX(points);
	Eigen::MatrixXd  Y = GetY(points);

	Eigen::MatrixXd A = UT * U;

	if(A.determinant() == 0) 
	{
		std::cout << "A determinant is zero\n";
		retVal = false;
	}

	Eigen::MatrixXd B = A.inverse();
	Eigen::MatrixXd C = MInv * B;
	Eigen::MatrixXd D = C * UT;
	Eigen::MatrixXd E = D * X;
	Eigen::MatrixXd F = D * Y;

	for(int i = 0; i < 4; i++)
	{
		double x = E(i, 0);
		double y = F(i, 0);

		MyPoint p(x, y);
		P[i] = p;
	}
	return retVal;
}

Eigen::MatrixXd CVSystem::CurveFitting::GetX(std::vector<CVSystem::MyPoint> points)
{
	Eigen::MatrixXd X(points.size(), 1);

	for(size_t i = 0; i < points.size(); i++)
		X.row(i)(0) = points[i].x;
	
	return X;
}

Eigen::MatrixXd CVSystem::CurveFitting::GetY(std::vector<CVSystem::MyPoint> points)
{
	Eigen::MatrixXd Y(points.size(), 1);

	for(size_t i = 0; i < points.size(); i++)
		Y.row(i)(0) = points[i].y;

	return Y;
}

Eigen::MatrixXd CVSystem::CurveFitting::GetU(std::vector<CVSystem::MyPoint> points)
{
	std::vector<double> npls = NormalizedPathLengths(points);

	Eigen::MatrixXd U(npls.size(), 4);
	
	for(size_t i = 0; i < npls.size(); i++)
	{
		U.row(i)(0) = pow(npls[i], 3); 
		U.row(i)(1) = pow(npls[i], 2); 
		U.row(i)(2) = pow(npls[i], 1); 
		U.row(i)(3) = pow(npls[i], 0); 
	}

	return U;
}

std::vector<double> CVSystem::CurveFitting::NormalizedPathLengths(std::vector<CVSystem::MyPoint> points)
{
	std::vector<double> pathLength(points.size());
	std::vector<double> zpl(points.size());

	pathLength[0] = 0;

	for(size_t i = 1; i < points.size(); i++)
	{
		MyPoint p1 = points[i];
		MyPoint p2 = points[i-1];
		double distance = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
		pathLength[i] += pathLength[i-1] + distance;
	}

	
	for(size_t i = 0; i < zpl.size(); i++)
		{ zpl[i] = pathLength[i] / pathLength[pathLength.size()-1]; }

	return zpl;
}

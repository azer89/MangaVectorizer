
/**
 * Bezier curve fitting using least square (http_://jimherold.com/2012/04/20/least-squares-bezier-fit/)
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __Curve_Fitting__
#define __Curve_Fitting__

#include <vector>
#include <Eigen/Dense>
#include "MyPoint.h"

namespace CVSystem
{
	class CurveFitting
	{
	public:
		// fit bezier curve to a set of points
		static bool PointstoBezier(std::vector<MyPoint> points, std::vector<MyPoint>& P);

	private:
		// calculate U matrix
		static Eigen::MatrixXd GetU(std::vector<MyPoint> points);

		// calculate X matrix
		static Eigen::MatrixXd GetX(std::vector<MyPoint> points);

		// calculate Y matrix
		static Eigen::MatrixXd GetY(std::vector<MyPoint> points);

		// normalize point set
		static std::vector<double> NormalizedPathLengths(std::vector<MyPoint> points);
	};
}

#endif

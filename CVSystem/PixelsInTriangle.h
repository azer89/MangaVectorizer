
/**
 * Calculating pixels inside a triangle
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __Pixels_In_Triangles__
#define __Pixels_In_Triangles__

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "MyPoint.h"

namespace CVSystem
{
	struct PixelsInTriangles
	{
	public:
		// median value of pixels
		static int MedianValue(cv::Mat inpImg, CVSystem::MyPoint p0, CVSystem::MyPoint p1, CVSystem::MyPoint p2);

	private:
		// bounding box
		static void GetMaxMin(std::vector<double> xs, std::vector<double> ys, double& xmax, double& xmin, double& ymax, double& ymin);
	};
}
#endif
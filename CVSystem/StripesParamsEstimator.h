
/**
 * Estimating stripes pattern properties: thickness, orientation, and spacing
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __LineParamsEstimator__
#define __LineParamsEstimator__

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "ImageThinning.h"
#include "UtilityFunctions.h"

class StripesParamsEstimator
{
public:
	// Calculate parameters
	static void TestParams(cv::Mat img, cv::Mat mask, double& orientationParam, double& thicknessParam, double& gapParam);

	static int pad_size;		
	static int area_threshold;

private:
	// number rounding
	static double round(double number);

	// debug
	static void  DisplayDistance(cv::Mat src, std::string title);

	// calculate orientation
	static double LineOrientation(cv::Point2f p0, cv::Point2f p1);
};

#endif

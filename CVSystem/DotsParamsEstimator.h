
/**
 * Estimating dots pattern properties: radius, orientation, and spacing
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __DotParamsEstimation__
#define __DotParamsEstimation__

#include <vector>

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "PointCloud.h"

// 
class DotParamsEstimator
{
public:
	// dots pattern has max orientation 90 degree
	static int orientation_max;

	// morphological operation
	static int    morph_elem_size;
	static int    morph_erode_num;
	static double morph_mask_scale;	

	// max sampling
	static int	  dots_max_sample;

	// calculate parameters
	static void TestParams(cv::Mat img, cv::Mat mask, double& orientationParam, double& radiusParam, double& gapParam);

private:
	// number rounding
	static double round(double number);

	// get circles
	static void DetectCircles(cv::Mat img, std::vector<cv::Point2f>& centers, std::vector<double>& rads, std::vector<cv::Moments>& moms);

	// calculate orientation and spacing
	static void GetOrientationsAndGaps(std::vector<cv::Point2f> queryPts, std::vector<cv::Point2f> centers, std::vector<double>& orientationList, std::vector<double>& gapList);

	// orientation of a line defined by two points
	static double LineOrientation(cv::Point2f p0, cv::Point2f p1);
};

#endif


/**
 * Estimating grid pattern properties: side length, orientation, and spacing
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __GridParamsEstimation__
#define __GridParamsEstimation__

#include "stdafx.h"
#include "NanoFLANN.h"
#include "PointCloud.h"

class GridParamsEstimator
{
public:
	// Calculate parameters
	static void TestParams(cv::Mat img, cv::Mat mask, double& orientationParam, double& lengthParam, double& spacingParam);
	
private:
	// Morphological operations
	static int    morph_elem_size;
	static int    morph_erode_num;
	static double morph_mask_scale;

	// sampling number
	static int	  dots_max_sample;

	// max orientation for grid pattern is 90
	static int	  orientation_max;

	// orientation binning
	static double bin_size;

	// calculate orientation and spacing
	static void   GetOrientationsAndSpacing(std::vector<cv::Point2f> queryPts, std::vector<cv::Point2f> centers, std::vector<double>& orientationList, std::vector<double>& gapList);

	// orientation of a line defined by two points
	static double LineOrientation(cv::Point2f p0, cv::Point2f p1);

	// number rounding
	static double round(double number);
};

#endif


/**
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __ImageThinning__
#define __ImageThinning__

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class ImageThinning
{
public:
	// proxy to zhangsuen thinning
	static void ZhangSuenThinningIteration(cv::Mat& img, int iter);

	// zhangsuen thinning
	static void ZhanSuenThinning(const cv::Mat& src, cv::Mat& dst);
};

#endif

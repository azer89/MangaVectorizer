
//! \author philipp <bytefish[at]gmx[dot]de>
//! \copyright BSD, see LICENSE.

/**
 * Local Binary Pattern
 * http://bytefish.de/blog/local_binary_patterns/
 *
 * Modified by: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#ifndef __Local_Binary_Pattern__
#define __Local_Binary_Pattern__

#include <cv.h>
#include <limits>

using namespace cv;
using namespace std;

//#ifndef  M_PI
//#define  M_PI 3.14159265358979323846264338327950288
//#endif

namespace lbp {

	// Template functions
	template <typename _Tp>
	void OLBP_(const cv::Mat& src, cv::Mat& dst);

	template <typename _Tp>
	void ELBP_(const cv::Mat& src, cv::Mat& dst, int radius = 1, int neighbors = 8);

	template <typename _Tp>
	void VARLBP_(const cv::Mat& src, cv::Mat& dst, int radius = 1, int neighbors = 8);

	// wrapper functions

	// Original LBP
	void OLBP(const Mat& src, Mat& dst);

	// Extended LBP
	void ELBP(const Mat& src, Mat& dst, int radius = 1, int neighbors = 8);

	// Var LBP
	void VARLBP(const Mat& src, Mat& dst, int radius = 1, int neighbors = 8);

	// Mat return type functions

	// Original LBP
	Mat OLBP(const Mat& src);

	// Extended LBP
	Mat ELBP(const Mat& src, int radius = 1, int neighbors = 8);

	// Var LBP
	Mat VARLBP(const Mat& src, int radius = 1, int neighbors = 8);

}
#endif

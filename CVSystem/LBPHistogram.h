
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

#ifndef __LBP_Histogram__
#define __LBP_Histogram__

#include <cv.h>
#include <limits>

using namespace cv;
using namespace std;

namespace lbp 
{

	// Calculate histogram using mask (screentone regions only)
	void histogramMask(const Mat& src, Mat& hist, Mat& mask, int numPatterns);
	template <typename _Tp>
	void histogramMask_(const Mat& src, Mat& hist, Mat& mask, int numPatterns);


	// templated functions
	template <typename _Tp>
	void histogram_(const Mat& src, Mat& hist, int numPatterns);

	template <typename _Tp>
	double chi_square_(const Mat& histogram0, const Mat& histogram1);

	// non-templated functions
	void spatial_histogram(const Mat& src, Mat& spatialhist, int numPatterns, const Size& window, int overlap=0);

	// wrapper functions
	void spatial_histogram(const Mat& src, Mat& spatialhist, int numPatterns, int gridx=8, int gridy=8, int overlap=0);
	void histogram(const Mat& src, Mat& hist, int numPatterns);

	
	// Chi square distance
	double chi_square(const Mat& histogram0, const Mat& histogram1);

	// Mat return type functions
	Mat histogram(const Mat& src, int numPatterns);
	Mat spatial_histogram(const Mat& src, int numPatterns, const Size& window, int overlap=0);
	Mat spatial_histogram(const Mat& src, int numPatterns, int gridx=8, int gridy=8, int overlap=0);
}
#endif

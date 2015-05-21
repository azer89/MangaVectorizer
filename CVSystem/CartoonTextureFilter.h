
/** 
 * This is a proxy class to cartoon+texture filter
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#ifndef __Cartoon_Texture_Filter__
#define __Cartoon_Texture_Filter__

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class CartoonTextureFilter
{
public:
	// Do segmentation using cartoon+texture filter
	// The input is raster manga object
	// the output is mask image
	static cv::Mat DoSegmentation(cv::Mat img);

};

#endif
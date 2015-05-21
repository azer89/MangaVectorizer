
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "CartoonTextureFilter.h"
#include "LibCartoon.h"
#include "SystemParams.h"

cv::Mat CartoonTextureFilter::DoSegmentation(cv::Mat img)
{
	cv::Mat inpImg;
	img.convertTo(inpImg, CV_8UC1);

	int img_width = inpImg.cols;
	int img_height = inpImg.rows;

	double f_sigma = SystemParams::ct_f_sigma;
	double v_lim = SystemParams::ct_v_lim;

	float* inData  = (float*)malloc(img_width * inpImg.rows * sizeof(float));
	float* outData = (float*)malloc(img_width * inpImg.rows * sizeof(float));
	for(int a = 0; a < img_width; a++)
		for(int b = 0; b < inpImg.rows; b++)
			{ inData[a + b * inpImg.cols] = inpImg.ptr<uchar>(b, a)[0]; }

	non_linear_cartoon(inData, outData, f_sigma,  inpImg.cols,  inpImg.rows);

	cv::Mat outImg = cv::Mat::zeros(inpImg.size(), CV_8UC1);
	for(int a = 0; a < img_width; a++)
	{
		for(int b = 0; b < outImg.rows; b++)
		{
			int idx = a + b * outImg.cols;
			float fValue = inData[idx] - outData[idx];
			fValue =  (fValue + v_lim) * 255.0f / (2.0f * v_lim);

			if(fValue < 0 || fValue > 255)
				{ outImg.ptr<uchar>(b, a)[0] = 255; }
		}
	}

	cv::Mat elem = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
	cv::morphologyEx(outImg, outImg, cv::MORPH_CLOSE, elem);	
	cv::morphologyEx(outImg, outImg, cv::MORPH_ERODE, elem);
	cv::morphologyEx(outImg, outImg, cv::MORPH_CLOSE, elem);	
	cv::morphologyEx(outImg, outImg, cv::MORPH_ERODE, elem);

	return outImg;
}
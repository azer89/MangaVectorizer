
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include <iostream>
#include "StripesParamsEstimator.h"

int StripesParamsEstimator::pad_size = 5;
int StripesParamsEstimator::area_threshold = 100;

double StripesParamsEstimator::round(double number) { return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5); }

// Using median
void StripesParamsEstimator::TestParams(cv::Mat img, cv::Mat mask, double& orientationParam, double& thicknessParam, double& gapParam)
{
	cv::Mat inpImg;
	cv::Mat maskImg;
	img.copyTo(inpImg);
	mask.copyTo(maskImg);

	// Binary Images
	cv::Mat binImg; cv::Mat binImgInv;
	cv::threshold(inpImg, binImg,    127, 255, CV_THRESH_BINARY);
	cv::threshold(inpImg, binImgInv, 127, 255, CV_THRESH_BINARY_INV);

	// Apply mask
	for(int a = 0; a < maskImg.cols; a++)
		for(int b = 0; b < maskImg.rows; b++)
			if(maskImg.ptr<uchar>(b, a)[0] < 127)
				binImg.at<uchar>(b, a) = 0;

	// Thinning
	cv::Mat thinA; 
	cv::Mat thinB;
	ImageThinning::ZhanSuenThinning(binImgInv, thinA); 
	ImageThinning::ZhanSuenThinning(binImg, thinB);
	//cv::imshow("thinA", thinA);
	//cv::imshow("thinB", thinB);

	std::vector<int> orientationList;
	std::vector<int> thicknessList;
	std::vector<int> gapList;

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(thinA, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	if(contours.size() > 0)
	{
		//cv::RNG rng(12345); //
		for(size_t i = 0; i < contours.size(); i++) 
		{
			if(cv::arcLength(contours[i], false) < StripesParamsEstimator::area_threshold) continue;

			cv::Vec4f line;
			cv::fitLine(contours[i],line, CV_DIST_L2, 0, 0.01, 0.01);
			double mult = std::max(thinA.rows, thinA.cols);
			cv::Point startPoint(line[2] - mult * line[0], line[3] - mult * line[1]);
			cv::Point endPoint(  line[2] + mult * line[0], line[3] + mult * line[1]);

			// should be rounded !
			int orient = round(LineOrientation(startPoint, endPoint));
			if(orient < 0){orient = 180 + orient;}
			orientationList.push_back(orient);
		}
	}

	// DISTANCE
	cv::Mat distImgA; cv::Mat distImgB;
	cv::distanceTransform(binImgInv, distImgA, CV_DIST_L2, CV_DIST_MASK_PRECISE);
	cv::distanceTransform(binImg, distImgB,    CV_DIST_L2, CV_DIST_MASK_PRECISE);

	for(int a = 0; a < thinA.cols; a++)
	{
		for(int b = 0; b < thinA.rows; b++)
		{
			if(thinA.ptr<uchar>(b, a)[0] > 0)
			{
				int val = distImgA.ptr<float>(b, a)[0];
				if(val % 2 == 0) val *= 2;
				else val = val * 2 - 1;
				thicknessList.push_back(val);
			}
		}
	}

	for(int a = 0; a < thinB.cols; a++)
	{
		for(int b = 0; b < thinB.rows; b++)
		{
			if(thinB.ptr<uchar>(b, a)[0] > 0)
			{
				int val = distImgB.ptr<float>(b, a)[0];
				if(val % 2 == 0) val *= 2;
				else val = val * 2 - 1;
				gapList.push_back(val);
			}
		}
	}
	std::sort(orientationList.begin(), orientationList.end());
	std::sort(thicknessList.begin(), thicknessList.end());
	std::sort(gapList.begin(), gapList.end());

	orientationParam = orientationList[orientationList.size() / 2];
	thicknessParam = thicknessList[thicknessList.size() / 2];
	gapParam = gapList[gapList.size() / 2];
	gapParam += thicknessParam;

	std::cout << "thickness   : " << thicknessParam   << "\n";   //
	std::cout << "gap         : " << gapParam         << "\n";   //
	std::cout << "orientation : " << orientationParam << "\n";   //
}


double StripesParamsEstimator::LineOrientation(cv::Point2f p0, cv::Point2f p1)
{
	cv::Point2f dirA = (p1 - p0);
	double length = sqrt(dirA.x * dirA.x + dirA.y * dirA.y);
	dirA.x = dirA.x / length;
	dirA.y = dirA.y / length;

	cv::Point2f dirX(1, 0);

	double perpDot = dirX.x * dirA.y - dirX.y * dirA.x;
	return  (float)atan2(perpDot, (double)dirX.dot(dirA)) * 180.0 / M_PI;

	/*

	This one has bug?

	double x = abs(p0.x - p1.x);
	double y = p1.y - p0.y;

	double orient = atan(y / x) * 180.0 / M_PI;
	if(orient < 0) orient = 180 + orient;

	return orient;
	*/
}

void StripesParamsEstimator::DisplayDistance(cv::Mat src, std::string title)
{
	double minVal; double maxVal;
	cv::minMaxLoc( src, &minVal, &maxVal, 0, 0, cv::Mat() );
	minVal = abs(minVal); maxVal = abs(maxVal);

	cv::Mat drawing = cv::Mat::zeros( src.size(), CV_8UC3 );
	for( int j = 0; j < src.rows; j++ )
	{ 
		for( int i = 0; i < src.cols; i++ )
		{
			double d = (src.at<float>(j,i) - minVal) / (maxVal - minVal);
			//int color = 0;
			//if(d > 0) color = 255;
			//drawing.at<cv::Vec3b>(j,i) = cv::Vec3b(0, 0, color);
			drawing.at<cv::Vec3b>(j,i) = cv::Vec3b(0, 0, d * 255.0);

		}
	}
	cv::imshow( title, drawing );
}


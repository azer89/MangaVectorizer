
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include <iostream>
#include "DotsParamsEstimator.h"

int DotParamsEstimator::orientation_max = 45;

int DotParamsEstimator::morph_elem_size = 7;
int DotParamsEstimator::morph_erode_num = 3;
double DotParamsEstimator::morph_mask_scale = 0.25;

int DotParamsEstimator::dots_max_sample = 100;

double DotParamsEstimator::round(double number) { return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5); }

void DotParamsEstimator::TestParams(cv::Mat img, cv::Mat mask, double& orientationParam, double& radiusParam, double& gapParam)
{
	cv::Mat inpImg;
	img.copyTo(inpImg);

	for(int a = 0; a < inpImg.cols; a++)
		for(int b = 0; b < inpImg.rows; b++)
			if(mask.ptr<uchar>(b, a)[0] < 127)
				{ inpImg.at<uchar>(b, a) = 255; }

	cv::resize(mask, mask, cv::Size(mask.cols * morph_mask_scale, mask.rows * morph_mask_scale), cv::INTER_NEAREST);
	cv::Mat elem = cv::getStructuringElement(CV_SHAPE_ELLIPSE, cv::Size(morph_elem_size, morph_elem_size));
	for(int a = 0; a < morph_erode_num; a++)
		{ cv::morphologyEx(mask, mask, cv::MORPH_ERODE, elem); }
	cv::resize(mask, mask, img.size(), cv::INTER_NEAREST);

	std::vector<double> radiusList;
	std::vector<cv::Moments> momList;
	std::vector<cv::Point2f> centerList;
	std::vector<cv::Point2f> insideCenterList;

	// Detect every circles
	DetectCircles(inpImg, centerList, radiusList, momList);

	cv::Mat drawing = cv::Mat::zeros(inpImg.size(), CV_8UC3);
	for(size_t a = 0; a < centerList.size(); a++)
		{ cv::circle(drawing, centerList[a], 1, cv::Scalar(0, 0, 255), -1); }

	for(size_t a = 0; a < centerList.size(); a++)
	{
		cv::Point2f pt = centerList[a];
		if(mask.ptr<uchar>((int)pt.y, (int)pt.x)[0] > 127 )
			{ insideCenterList.push_back(pt); }
	}

	//for(size_t a = 0; a < centerList.size(); a++)
	//	if(mask.ptr<uchar>((int)centerList[a].y, (int)centerList[a].x)[0] > 127 )
	//		{ insideCenterList.push_back(centerList[a]); }

	std::vector<int> indices(insideCenterList.size());
	for(size_t a = 0; a < insideCenterList.size(); a++)
		{ indices[a] = a; }
	random_shuffle(std::begin(indices), std::end(indices));

	//std::vector<cv::Point2f> sampleCenters(dots_max_sample);
	//for(int a = 0; a < dots_max_sample; a++)
	//	{ sampleCenters[a] = insideCenterList[indices[a]]; }
	//// Sampling
	int maxSample = dots_max_sample;
	if(maxSample < insideCenterList.size())
	{ maxSample = insideCenterList.size(); }
	std::vector<cv::Point2f> sampleCenters(maxSample);
	for(int a = 0; a < maxSample; a++)
	{ sampleCenters[a] = insideCenterList[indices[a]]; }

	std::vector<double> orientationList;
	std::vector<double> gapList;
	GetOrientationsAndGaps(sampleCenters, centerList, orientationList, gapList);

	std::sort(orientationList.begin(), orientationList.end());
	std::sort(radiusList.begin(), radiusList.end());
	std::sort(gapList.begin(), gapList.end());

	orientationParam = orientationList[orientationList.size() / 2];
	radiusParam = radiusList[radiusList.size() / 2];
	gapParam = gapList[gapList.size() / 2];
}

double DotParamsEstimator::LineOrientation(cv::Point2f p0, cv::Point2f p1)
{
	double x = abs(p0.x - p1.x);
	double y = abs(p0.y - p1.y);
	return atan(y / x) * 180.0 / M_PI;
}

void DotParamsEstimator::GetOrientationsAndGaps(std::vector<cv::Point2f> queryPts, std::vector<cv::Point2f> centers, std::vector<double>& orientationList, std::vector<double>& gapList)
{
	using namespace nanoflann;

	orientationList.clear();
	gapList.clear();

	PointCloud<float> pointCloud;
	pointCloud.pts.resize(centers.size());
	for(size_t a = 0; a < centers.size(); a++)
	{
		pointCloud.pts[a].x = centers[a].x;
		pointCloud.pts[a].y = centers[a].y;
	}

	typedef KDTreeSingleIndexAdaptor< L2_Simple_Adaptor<float, PointCloud<float> >, PointCloud<float>, 2 /*dim*/> my_kd_tree_t;
	my_kd_tree_t   index(2 /*dim*/, pointCloud, KDTreeSingleIndexAdaptorParams(10000 /* max leaf */) );
	index.buildIndex();

	size_t num_results = 5;
	for(size_t a = 0; a < queryPts.size(); a++)
	{
		float query_pt[2] = { queryPts[a].x, queryPts[a].y};
		std::vector<size_t>   ret_index(num_results);
		std::vector<float> out_dist_sqr(num_results);
		index.knnSearch(&query_pt[0], num_results, &ret_index[0], &out_dist_sqr[0]);

		for (size_t i = 0; i < num_results; i++)
		{
			double dist = out_dist_sqr[i];
			if(dist > M_EPS)
			{
				int retIdx = ret_index[i];
				cv::Point2f resPt(pointCloud.pts[retIdx].x, pointCloud.pts[retIdx].y);
				
				//// BUG: orientation is sometimes wrong
				double orientVal = LineOrientation(queryPts[a], resPt);
				if(orientVal > orientation_max)
					{ orientVal = fmod(orientVal, (double)orientation_max); }
				orientationList.push_back(orientVal);

				gapList.push_back(dist);
			}
		}
	}
}

void DotParamsEstimator::DetectCircles(cv::Mat img, std::vector<cv::Point2f>& centers, std::vector<double>& rads, std::vector<cv::Moments>& moms)
{
	centers.clear();
	rads.clear();
	moms.clear();

	cv::Mat inpImg, distance, peaks;

	std::vector<std::vector<cv::Point>> contours;
	inpImg = img.clone();

	cv::threshold(inpImg, inpImg, 127, 255, cv::THRESH_BINARY_INV);
	cv::distanceTransform(inpImg,distance,CV_DIST_L2,CV_DIST_MASK_5);

	// In order to find the local maxima, "distance" is subtracted from the result of the dilatation of "distance". 
	// All the peaks keep the save value
	cv::dilate(distance, peaks,  cv::Mat(), cv::Point(-1,-1), 3);
	cv::dilate(inpImg,   inpImg, cv::Mat(), cv::Point(-1,-1), 3);

	// Now all the peaks should be exactly 0
	peaks = peaks - distance;

	// And the non-peaks 255
	cv::threshold(peaks, peaks, 0, 255, cv::THRESH_BINARY);
	peaks.convertTo(peaks, CV_8U);

	// Only the zero values of "peaks" that are non-zero in "objects" are the real peaks
	cv::bitwise_xor(peaks, inpImg, peaks);

	// The peaks that are distant from less than 2 pixels are merged by dilatation
	cv::dilate(peaks,peaks,cv::Mat(),cv::Point(-1,-1),1);

	// In order to map the peaks, findContours() is used. The results are stored in "contours"
	cv::findContours(peaks, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	// The next steps are applied only if, at least, one contour exists
	if(contours.size()>0)
	{		
		// Calculates the moments of each peak and then the center of the peak
		// which are approximatively the center of each objects of interest
		for(size_t i=0;i<contours.size();i++) 
		{
			cv::Moments mom = cv::moments(contours[i]);
			cv::Point2f center(mom.m10 / mom.m00, mom.m01 / mom.m00);	
			if(center.x > 0 && center.y > 0)
			{
				rads.push_back((float) (distance.at<float>((int)center.y, (int) center.x)+1));
				centers.push_back(center);
				moms.push_back(mom);
			}
		}       
	}
}

/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include <iostream>
#include "GridParamsEstimator.h"

int		GridParamsEstimator::morph_elem_size = 3;
int		GridParamsEstimator::morph_erode_num = 3;
double  GridParamsEstimator::morph_mask_scale = 0.25;

int		GridParamsEstimator::dots_max_sample = 100;
int		GridParamsEstimator::orientation_max = 45;
double  GridParamsEstimator::bin_size = 3;

double GridParamsEstimator::round(double number) { return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5); }

void GridParamsEstimator::TestParams(cv::Mat img, cv::Mat mask, double& orientationParam, double& lengthParam, double& spacingParam)
{
	cv::Mat inpImg;
	img.copyTo(inpImg);

	//// prepare mask
	cv::resize(mask, mask, cv::Size(mask.cols * morph_mask_scale, mask.rows * morph_mask_scale), cv::INTER_NEAREST);
	cv::Mat elem = cv::getStructuringElement(CV_SHAPE_ELLIPSE, cv::Size(morph_elem_size, morph_elem_size));
	for(int a = 0; a < morph_erode_num; a++)
	{ cv::morphologyEx(mask, mask, cv::MORPH_ERODE, elem); }
	cv::resize(mask, mask, img.size(), cv::INTER_NEAREST);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::Mat inpImgClone = inpImg.clone();
	findContours( inpImgClone, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

	
	//// begin to detect rectangles
	std::vector<cv::Point2f> centerList;
	std::vector<double> areaList;
	std::vector<cv::Point2f> insideCenterList;

	if(contours.size() > 0)
	{
		////~~~~cv::Mat cntrImg = cv::Mat::zeros( inpImg.size(), CV_8UC3 );
		cv::RNG rng(12345);
		for( int i = 0; i< contours.size(); i++ )
		{
			double areaSize = cv::contourArea(contours[i]);

			if(areaSize > 5)
			{
				cv::Moments mom = cv::moments(contours[i]);
				cv::Point2f ctr(mom.m10 / mom.m00, mom.m01 / mom.m00);	
				centerList.push_back(ctr);
				areaList.push_back(areaSize);

				////~~~~cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
				////~~~~drawContours( cntrImg, contours, i, color, 2, 8, hierarchy, 0, cv::Point() );
			}
		}
		////~~~~cv::imshow(" Contour", cntrImg);
	}

	////~~~~cv::imshow(" Mask", mask);
	////~~~~cv::imshow(" Pattern", inpImg);	

	//// Inside center
	for(size_t a = 0; a < centerList.size(); a++)
	{
		cv::Point2f pt = centerList[a];
		if(mask.ptr<uchar>((int)pt.y, (int)pt.x)[0] > 127 )
		{ insideCenterList.push_back(pt); }
	}

	//// Shuffling
	std::vector<int> indices(insideCenterList.size());
	for(size_t a = 0; a < insideCenterList.size(); a++)
	{ indices[a] = a; }
	random_shuffle(std::begin(indices), std::end(indices));

	//// Sampling
	int maxSample = dots_max_sample;
	if(maxSample < insideCenterList.size())
		{ maxSample = insideCenterList.size(); }
	std::vector<cv::Point2f> sampleCenters(maxSample);
	for(int a = 0; a < maxSample; a++)
	{ sampleCenters[a] = insideCenterList[indices[a]]; }

	std::vector<double> orientationList;
	std::vector<double> spacingList;
	GetOrientationsAndSpacing(sampleCenters, centerList, orientationList, spacingList);

	std::sort(orientationList.begin(), orientationList.end());
	std::sort(areaList.begin(), areaList.end());
	std::sort(spacingList.begin(), spacingList.end());

	orientationParam = orientationList[orientationList.size() / 2];
	lengthParam = sqrt(areaList[areaList.size() / 2]);
	lengthParam /= 2.0;
	spacingParam = spacingList[spacingList.size() / 2];

	std::cout << "orientation: " << orientationParam << "\n";
	std::cout << "length     : " << lengthParam << "\n";
	std::cout << "spacing    : " << spacingParam << "\n";

	////~~~~cv::Mat centerImg = cv::Mat::zeros( inpImg.size(), CV_8UC3 );
	////~~~~centerImg = 0;
	////~~~~for(int a = 0; a < insideCenterList.size(); a++)
	////~~~~{
	////~~~~	cv::Point2f pt = insideCenterList[a];
	////~~~~	cv::circle(centerImg, pt, 2, cv::Scalar(0, 0, 255), -1);
	////~~~~}
	////~~~~cv::imshow(" Centers", centerImg);

}


void GridParamsEstimator::GetOrientationsAndSpacing(std::vector<cv::Point2f> queryPts, std::vector<cv::Point2f> centers, std::vector<double>& orientationList, std::vector<double>& gapList)
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

				double orientVal = LineOrientation(queryPts[a], resPt);

				double divVal = round(orientVal / bin_size);
				orientVal = divVal * bin_size;

				if(orientVal > orientation_max)
				{ orientVal = fmod(orientVal, (double)orientation_max); }
				orientationList.push_back(orientVal);
				
				gapList.push_back(dist);
			}
		}
	}
}

double GridParamsEstimator::LineOrientation(cv::Point2f p0, cv::Point2f p1)
{
	double x = abs(p0.x - p1.x);
	double y = abs(p0.y - p1.y);
	return atan(y / x) * 180.0 / M_PI;
}

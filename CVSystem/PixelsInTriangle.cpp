
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "PixelsInTriangle.h"

void CVSystem::PixelsInTriangles::GetMaxMin(std::vector<double> xs, std::vector<double> ys, double& xmax, double& xmin, double& ymax, double& ymin)
{
	xmax = DBL_MIN;
	xmin = DBL_MAX;

	for(size_t a = 0; a < xs.size(); a++)
	{
		double d = xs[a];
		if(d > xmax) xmax = d;
		if(d < xmin) xmin = d;
	}

	ymax = DBL_MIN;
	ymin = DBL_MAX;

	for(size_t a = 0; a < ys.size(); a++)
	{
		double d = ys[a];
		if(d > ymax) ymax = d;
		if(d < ymin) ymin = d;
	}
}

int CVSystem::PixelsInTriangles::MedianValue(cv::Mat inpImg, CVSystem::MyPoint p0, CVSystem::MyPoint p1, CVSystem::MyPoint p2)
{
	std::vector<double> xs;
	xs.push_back(p0.x);
	xs.push_back(p1.x);
	xs.push_back(p2.x);
	std::vector<double> ys;
	ys.push_back(p0.y);
	ys.push_back(p1.y);
	ys.push_back(p2.y);

	double xmax, xmin, ymax, ymin;
	GetMaxMin(xs, ys, xmax, xmin, ymax, ymin);

	cv::Mat roiImg = inpImg(cv::Rect(xmin, ymin, abs(xmin-xmax), abs(ymin-ymax)));

	if(roiImg.cols == 0 || roiImg.rows == 0)
		return -1;

	cv::Mat maskImg = cv::Mat::zeros(roiImg.size(), CV_8UC1);

	std::vector<cv::Point> tri(3);
	tri[0] = cv::Point(xs[0]-xmin, ys[0]-ymin);
	tri[1] = cv::Point(xs[1]-xmin, ys[1]-ymin);
	tri[2] = cv::Point(xs[2]-xmin, ys[2]-ymin);

	const cv::Point* elementPoints[1] = { &tri[0] };
	int numberOfPoints = (int)tri.size();

	cv::fillPoly (maskImg, elementPoints, &numberOfPoints, 1, cv::Scalar(255), 8);

	std::vector<int> pxVals;
	for(int a = 0; a < maskImg.cols; a++)
	{
		for(int b = 0; b < maskImg.rows; b++)
		{
			pxVals.push_back(roiImg.ptr<uchar>(b, a)[0]);
		}
	}

	std::sort(pxVals.begin(), pxVals.end());
	int val = pxVals[pxVals.size() / 2];
	return val;
}
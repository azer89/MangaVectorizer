
/**
 * Screentone segmentation class with two different approach:
 *	- Level set and gabor wavelet (Chan-Vese algorithm)
 *	- Cartoon+texture filter
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __Screentone_Segmentation__
#define __Screentone_Segmentation__

#include <vector>
// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "MyPoint.h"
#include "MyLine.h"

#include "GaborManjunathMa.h"
#include "RunningStat.h"


namespace CVSystem
{
	class ScreentoneSegmentation
	{
	public:
		// Constructor
		ScreentoneSegmentation();

		// Destructor
		~ScreentoneSegmentation();

		// Cartoon+texture segmentation
		void ComputeCTSegmentation();

		// level set segmentation
		void ComputeLSSegmentation(std::vector<std::vector<CVSystem::MyLine>> sLines);	

		// DIstance map (;evel set)
		void ComputeDistanceMap(std::vector<std::vector<CVSystem::MyLine>> sLines);

		// init local variables
		void Init(std::string filename, int width, int height);

		// Get label map
		int* GetLabelMap() { return labelMap; }

		// get dilated label map
		int* GetDilatedLabelMap() { return dilatedLabelMap; }

	private:
		// refine segmentation (cartoon+texture filter)
		void RemoveSmallArea1(cv::Mat& segm);

		// refine segmentation (cartoon+texture filter)
		void RemoveSmallArea2(cv::Mat& segm);

		// gabor wavelet feature (level set)
		std::vector<double> GetStrokeFeature(std::vector<MyPoint> sPts);

		// gabor wavelet feature (level set)
		std::vector<double> GetPixelFeature(MyPoint p);
		
		// get distance of two features (level set)
		double GetDistance(std::vector<double> f1, std::vector<double> f2);

		// get distance of two features (level set)
		double GetDistance(std::vector<double> fv, int x, int y);

		// distance map (level set)
		void   GetDistanceMap(std::vector<double> fv);		

		// gabor wavelet feature (level set) 
		void   BuildFeature();
		
		// standard deviation to construct gabor wavelet feature (level set)
		void FeatureStdDev(cv::Mat fv, std::vector<double>& stds);	// one pass implementation		

	private:
		int* labelMap;
		int* dilatedLabelMap;

		GaborManjunathMa* gbr;

		std::vector<CVSystem::MyPoint> sPts;
		std::vector<double> stds;
		cv::Mat gbrImg;

		cv::Mat featureMatrix;
		std::string filename;
		cv::Mat inpImg;
		cv::Mat oriInpImg;

		int	img_ori_width;
		int img_ori_height;
		int	img_width_scale;
		int img_height_scale;

		bool shouldCalc;
	};
}

#endif

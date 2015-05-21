
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "ScreentoneClassifier.h"

#include "Dirent.h"
#include "UtilityFunctions.h"
#include "LBP.h"
#include "LBPHistogram.h"
#include "SystemParams.h"
#include "DotsParamsEstimator.h"
#include "StripesParamsEstimator.h"
#include "GridParamsEstimator.h"

#include <boost/chrono.hpp>

std::vector<double> CVSystem::ScreentoneClassifier::GetParameters(std::string fileName, int* mask)
{
	std::vector<double> params; // return values
	if(ReadParams(params, fileName))
		return params;

	if(!tonesLoaded)
		LoadTones();

	int neighbors = SystemParams::lbp_neighbors;
	int radius = SystemParams::lbp_radius;

	cv::Mat img = cv::imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat inpImg;
	img.convertTo(inpImg, CV_8UC1);	

	cv::Mat maskImg = cv::Mat::zeros(inpImg.rows, inpImg.cols, CV_8UC1);
	for(int a = 0; a < maskImg.cols; a++)
	{
		for(int b = 0; b < maskImg.rows; b++)
		{
			int idx = mask[a + b * maskImg.cols];
			if(idx == -1) {}
			else { maskImg.ptr<uchar>(b, a)[0] = 255; }
		}
	}

	// Should be resized
	cv::Mat oriInpImg = inpImg.clone();
	cv::Mat oriMaskImg = maskImg.clone();
	cv::resize(maskImg, maskImg, cv::Size(maskImg.cols * scale_factor, maskImg.rows * scale_factor), cv::INTER_NEAREST);
	cv::resize(inpImg,  inpImg, cv::Size(inpImg.cols * scale_factor, inpImg.rows * scale_factor), cv::INTER_NEAREST);
	
	// Timing
	using namespace boost::chrono ;
	auto start1 = steady_clock::now() ;

	cv::Mat lbpImg;
	lbp::ELBP(inpImg, lbpImg, radius, neighbors); 

	cv::Mat hist;
	lbp::histogramMask(lbpImg, hist, maskImg, std::pow(2.0, neighbors));

	std::cout << "LBP Matching...\n";

	

	std::vector<PairData> items;
	for(size_t a = 0; a < scDataList.size(); a++)
	{
		double d = lbp::chi_square(hist, scDataList[a].hist);
		items.push_back(PairData(a, d));
	}

	UtilityFunctions::QuickSortMain(items);

	for(size_t a = 0; a < items.size(); a++)
		std::cout << scDataList[items[a].first].filename << "\n";


	int nParam = this->max_neighbor;
	if(nParam > scDataList.size()) nParam = scDataList.size();
	std::vector<PairData> toneHistogram((int)NUM_SCREENTONE);
	for(size_t a = 0; a < toneHistogram.size(); a++)
	{
		toneHistogram[a].first = a;
		toneHistogram[a].second = 0;
	}
	for(int a = 0; a < nParam; a++)
	{
		int idx = scDataList[items[a].first].GetType();
		toneHistogram[idx].second += 1;
	}

	UtilityFunctions::QuickSortMain(toneHistogram);
	int toneType = toneHistogram[toneHistogram.size() - 1].first;	
	
	// Timing
	auto dur1 = steady_clock::now() - start1 ;
	std::cout << "LBP time: " << duration_cast<milliseconds>(dur1).count() << " milliseconds\n" ;


	std::cout << "Estimating parameters...\n";

	// Timing
	using namespace boost::chrono ;
	auto start2 = steady_clock::now() ;

	if(toneType == 0)	//// Dots
	{
		std::cout << "dots\n";

		params.push_back(0);

		double orientation,  radius,  spacing;
		DotParamsEstimator::TestParams(oriInpImg, oriMaskImg, orientation, radius, spacing);

		//params.push_back(90 - orientation);
		params.push_back(90 - orientation);
		params.push_back(radius);
		params.push_back(spacing);
	}
	else if(toneType == 1) //// stripes
	{
		std::cout << "stripes\n";

		params.push_back(1);

		double orientation,  radius,  spacing;
		StripesParamsEstimator::TestParams(oriInpImg, oriMaskImg, orientation, radius, spacing);
		
		//if(orientation < 0){orientation = 180 + orientation;}		
		params.push_back(180 - orientation);
		
		//params.push_back(orientation);
		params.push_back(radius);
		params.push_back(spacing);
	}
	else if(toneType == 2) //// grid
	{
		std::cout << "grid\n";

		params.push_back(2);

		double orientation,  sideLength,  spacing;
		GridParamsEstimator::TestParams(oriInpImg, oriMaskImg, orientation, sideLength, spacing);

		params.push_back(90 - orientation);
		params.push_back(sideLength);
		params.push_back(spacing);
	}

	// Timing
	auto dur2 = steady_clock::now() - start2 ;
	std::cout << "Property Extraction time: " << duration_cast<milliseconds>(dur2).count() << " milliseconds\n" ;

	std::cout << "Done\n";

	WriteParams(params, fileName);

	return params;
}

bool CVSystem::ScreentoneClassifier::ReadParams(std::vector<double>& params, std::string filename)
{
	// Params from file
	std::vector<std::string> fullpath = UtilityFunctions::split(filename, '//');
	std::vector<std::string> nameArray = UtilityFunctions::split(fullpath[fullpath.size() - 1], '.');
	std::string cfgFilename = "TextureConfig/" + nameArray[0] + ".cfg";
		
	FILE* fp;
	if((fp = fopen(cfgFilename.c_str(), "rb")) != NULL)
	{
		std::cout << "Tone config found.\n";
		int dataSize = 4;
		double* inData = (double *) calloc(dataSize, sizeof(double));
		fread(inData, sizeof(double), dataSize, fp);
		
		for(int a = 0; a < dataSize; a++)
			params.push_back(inData[a]);

		fclose(fp);
		free(inData);
		return true;
	}
	std::cout << "Tone config not found.\n";
	return false;
}

void CVSystem::ScreentoneClassifier::WriteParams(std::vector<double> params, std::string filename)
{
	// Params from file
	std::vector<std::string> fullpath = UtilityFunctions::split(filename, '//');
	std::vector<std::string> nameArray = UtilityFunctions::split(fullpath[fullpath.size() - 1], '.');
	std::string cfgFilename = "TextureConfig/" + nameArray[0] + ".cfg";

	int dataSize = params.size();
	double* outData = (double *) calloc(dataSize, sizeof(double));
	for(size_t a = 0; a < params.size(); a++)
		outData[a] = params[a];

	FILE* fp = fopen(cfgFilename.c_str(), "wb");
	fwrite(outData, sizeof(double), dataSize, fp);
	fclose(fp);
	free(outData);
}

void CVSystem::ScreentoneClassifier::LoadTones()
{
	DIR *dp;
	struct dirent *ep;

	std::vector<std::string> toneStr;
	dp = opendir ("./TextureBank");
	if (dp != NULL)
	{
		while (ep = readdir (dp)) toneStr.push_back(ep->d_name);
		(void) closedir (dp);
	}
	else perror ("Couldn't open the directory");

	int neighbors = SystemParams::lbp_neighbors;
	int radius = SystemParams::lbp_radius;

	for(size_t a = 0; a < toneStr.size(); a++)
	{
		if(toneStr[a] == "." || toneStr[a] == "..") continue;

		std::string fileName = "TextureBank\\" + toneStr[a];	//

		cv::Mat img = cv::imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);	//
		cv::Mat inpImg;
		img.convertTo(inpImg, CV_8UC1);
					
		cv::Mat lbpImg;
		lbp::ELBP(inpImg, lbpImg, radius, neighbors); 

		cv::Mat hist;	//
		lbp::histogram(lbpImg, hist, std::pow(2.0, neighbors));

		ScreentoneData sc(toneStr[a], img, lbpImg, hist);
		scDataList.push_back(sc);
	}

	std::cout << scDataList.size() << " texture bank loaded\n";
	tonesLoaded = true;
}



/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"

#include <iostream>
#include <boost/chrono.hpp>

#include "ScreentoneSegmentation.h"

#include "ChanVese_IPOL.h"
#include "CartoonTextureFilter.h"
#include "UtilityFunctions.h"
#include "SystemParams.h"

// Constructor
CVSystem::ScreentoneSegmentation::ScreentoneSegmentation()	
{
	labelMap = 0;
	dilatedLabelMap = 0;
	shouldCalc = false;
	gbr = new GaborManjunathMa();
}

// Destructor
CVSystem::ScreentoneSegmentation::~ScreentoneSegmentation()
{
	if(gbr)				delete gbr;
	if(labelMap)		delete labelMap;
	if(dilatedLabelMap) delete dilatedLabelMap;
}

void CVSystem::ScreentoneSegmentation::ComputeDistanceMap(std::vector<std::vector<CVSystem::MyLine>> sLines)
{
	for(size_t a = 0; a < sLines.size(); a++)
	{
		for(size_t b = 0; b < sLines[a].size(); b++)
		{
			sLines[a][b].XA *= SystemParams::s_rescale_factor; sLines[a][b].YA *= SystemParams::s_rescale_factor;
			sLines[a][b].XB *= SystemParams::s_rescale_factor; sLines[a][b].YB *= SystemParams::s_rescale_factor;
		}
	}

	if(sLines.size() == 0)
	{
		std::cout << "No scribble\n";
		return;
	}

	// has gabor wavelet been computed?
	if(this->shouldCalc) 
	{ 
		std::cout << "Building features...\n";
		BuildFeature(); 
		std::cout << "Done\n";
	}

	sPts.clear();
	for(size_t b = 0; b < sLines[0].size(); b++)
	{

		sPts.push_back(CVSystem::MyPoint(sLines[0][b].XA, sLines[0][b].YA));
		if(b == sLines[0].size() - 1)
			{ sPts.push_back(CVSystem::MyPoint(sLines[0][b].XB, sLines[0][b].YB)); }
		/*std::vector<CVSystem::MyPoint> tempPts = 
			UtilityFunctions::Bresenham(sLines[0][b].XA, sLines[0][b].YA, 
										sLines[0][b].XB, sLines[0][b].YB);
		for(size_t c = 0; c < tempPts.size(); c++)
			sPts.push_back(tempPts[c]);*/
	}

	// Running K-Means
	std::vector<double> fv = GetStrokeFeature(sPts);	

	std::cout << "Calculate Gabor Distance...\n";
	GetDistanceMap(fv);
	std::cout << "Done\n";

	// Debug
	UtilityFunctions::DisplayImageDebug(gbrImg, 1.0, "distance_map");
}

void CVSystem::ScreentoneSegmentation::ComputeCTSegmentation()
{
	// Timing calculation
	using namespace boost::chrono ;
	auto start = steady_clock::now() ;

	// Reset
	for(int a = 0; a < this->img_ori_width; a++)
	{
		for(int b = 0; b < this->img_ori_height; b++)
		{
			labelMap[a + b * this->img_ori_width] = -1;
			dilatedLabelMap[a + b * this->img_ori_width] = -1;
		}
	}

	cv::Mat outSegm = CartoonTextureFilter::DoSegmentation(this->oriInpImg);

	RemoveSmallArea1(outSegm);
	RemoveSmallArea2(outSegm);

	cv::imwrite("segmentation_result.png", outSegm);

	// Copy to label map
	outSegm.convertTo(outSegm, CV_8UC1);
	for(int i = 0; i < outSegm.cols; i++)
	{
		for(int j = 0; j < outSegm.rows; j++)
		{
			if(outSegm.ptr<uchar>(j, i)[0] > 0)
			{
				labelMap[i + j * outSegm.cols] = 0;
			}
		}
	}

	// Dilated Label Map (NEED TO FIX THIS!)
	cv::Mat elem2 = cv::getStructuringElement(2, cv::Size(10, 10));
	//for(int a = 0; a < 3; a++)
	cv::morphologyEx(outSegm, outSegm, cv::MORPH_DILATE, elem2);

	// Dilated Label Map
	outSegm.convertTo(outSegm, CV_8UC1);
	for(int i = 0; i < outSegm.cols; i++)
	{
		for(int j = 0; j < outSegm.rows; j++)
		{
			if(outSegm.ptr<uchar>(j, i)[0] > 0)
			{
				dilatedLabelMap[i + j * outSegm.cols] = 0;
			}
		}
	}

	auto dur = steady_clock::now() - start ;
	auto msDur = duration_cast<milliseconds>(dur).count();
	std::cout << "Cartoon+Texture Segmentation time: " <<  msDur << " milliseconds\n" ;
}

void CVSystem::ScreentoneSegmentation::ComputeLSSegmentation(std::vector<std::vector<CVSystem::MyLine>> sLines)
{
	// Reset
	for(int a = 0; a < this->img_ori_width; a++)
	{
		for(int b = 0; b < this->img_ori_height; b++)
		{
			labelMap[a + b * this->img_ori_width] = -1;
			dilatedLabelMap[a + b * this->img_ori_width] = -1;
		}
	}
	for(size_t a = 0; a < sLines.size(); a++)
	{
		for(size_t b = 0; b < sLines[a].size(); b++)
		{
			sLines[a][b].XA *= SystemParams::s_rescale_factor; sLines[a][b].YA *= SystemParams::s_rescale_factor;
			sLines[a][b].XB *= SystemParams::s_rescale_factor; sLines[a][b].YB *= SystemParams::s_rescale_factor;
		}
	}

	double* vMap = (double*)malloc(img_width_scale * img_height_scale * sizeof(double));	
	double* phi = (double*)malloc(this->img_width_scale * this->img_height_scale * sizeof(double));
	
	// assign zeros
	for(int j = 0; j < img_height_scale; j++)
	{
		for(int i = 0; i < img_width_scale; i++)
		{
			phi[i + j * img_width_scale] = 0.0;
		}
	}
	// Initial level set from strokes
	/*for(size_t a = 0; a < sPts.size(); a++)
		phi[(int)(sPts[a].x + sPts[a].y * img_width)] = 5.0;*/

	// Initial level set from distance function
	for(int j = 0; j < img_height_scale; j++)
	{
		for(int i = 0; i < img_width_scale; i++)
		{
			if(gbrImg.ptr<float>(j, i)[0] < 0.1) phi[i + j * img_width_scale] = -5;
			else if(gbrImg.ptr<float>(j, i)[0] > 0.9) phi[i + j * img_width_scale] = 5;
		}
	}

	// Speed function
	for(int a = 0; a < img_width_scale; a++)
	{
		for(int b = 0; b < img_height_scale; b++)
		{
			vMap[a + b * img_width_scale] = gbrImg.ptr<float>(b, a)[0];
		}
	}

	// Run Level-Set Method
	ChanVese(phi, vMap, img_width_scale, img_height_scale);
	
	// Why I should normalize ?
	double* phiNorm = UtilityFunctions::NormalizePhi(phi, img_width_scale, img_height_scale);
	
	// Debug
	//UtilityFunctions::DisplayImageDebug(phiNorm, img_height_scale, img_width_scale, 0.5, "Normalized Phi");

	cv::Mat rawSegm = cv::Mat::zeros(img_height_scale, img_width_scale, CV_8UC1);
	for(int i = 0; i < img_width_scale; i++)
	{
		for(int j = 0; j < img_height_scale; j++)
		{
			if(phiNorm[i + j * img_width_scale] > 0) 
				rawSegm.ptr<uchar>(j, i)[0] = 255;
		}
	}

	// Doing morphological stuff
	//  (NEED TO FIX THIS!)
	cv::Mat outSegm;
	cv::Size sz(img_ori_width, img_ori_height);
	cv::resize(rawSegm, outSegm, sz, cv::INTER_NEAREST);

	RemoveSmallArea1(outSegm);
	RemoveSmallArea2(outSegm);

	//cv::Mat maskImg = GetMask(outSegm);
	//cv::imshow("mask_img.png", maskImg);
		
	// THIS SHOULD BE DELETED
	//cv::Mat elem1 = cv::getStructuringElement(2, cv::Size(7, 7));
	//cv::morphologyEx(outSegm, outSegm, cv::MORPH_DILATE, elem1);
	//cv::morphologyEx(outSegm, outSegm, cv::MORPH_DILATE, elem1);

	//cv::imwrite("segmentation_result.png", outSegm);

	// Copy to label map
	outSegm.convertTo(outSegm, CV_8UC1);
	for(int i = 0; i < outSegm.cols; i++)
	{
		for(int j = 0; j < outSegm.rows; j++)
		{
			if(outSegm.ptr<uchar>(j, i)[0] > 0)
				labelMap[i + j * outSegm.cols] = 0;
		}
	}
	// Dilated Label Map (NEED TO FIX THIS!)
	cv::Mat elem2 = cv::getStructuringElement(2, cv::Size(7, 7));
	//for(int a = 0; a < 1; a++)
	cv::morphologyEx(outSegm, outSegm, cv::MORPH_DILATE, elem2);

	// Dilated Label Map
	outSegm.convertTo(outSegm, CV_8UC1);
	for(int i = 0; i < outSegm.cols; i++)
	{
		for(int j = 0; j < outSegm.rows; j++)
		{
			if(outSegm.ptr<uchar>(j, i)[0] > 0)
				dilatedLabelMap[i + j * outSegm.cols] = 0;
		}
	}
	
	delete vMap;
	delete phi;
	delete phiNorm;
}

void CVSystem::ScreentoneSegmentation::RemoveSmallArea1(cv::Mat& segm)
{
	if(SystemParams::s_min_size_area == 0)
		return;
	
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(segm, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	
	for(size_t a = 0; a < contours.size(); a++)
	{
		double areaSize = cv::contourArea(contours[a]);
		if(areaSize < SystemParams::s_min_size_area)
			cv::drawContours(segm, contours, a, cv::Scalar(0), -1);
	}

	// bug ???
	for(int i = 0; i < segm.cols; i++)
		for(int j = 0; j < segm.rows; j++)
			{if(segm.ptr<uchar>(j, i)[0] > 0)
				{ segm.ptr<uchar>(j, i)[0] = 255; }}
}

void CVSystem::ScreentoneSegmentation::RemoveSmallArea2(cv::Mat& segm)
{
	if(SystemParams::s_min_size_area == 0)
		{ return; }

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(segm, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	for(size_t a = 0; a < contours.size(); a++)
	{
		double areaSize = cv::contourArea(contours[a]);
		if(areaSize < SystemParams::s_min_size_area)
			cv::drawContours(segm, contours, a, cv::Scalar(255), -1);
	}

	// bug ???
	for(int i = 0; i < segm.cols; i++)
		for(int j = 0; j < segm.rows; j++)
			{ if(segm.ptr<uchar>(j, i)[0] > 0)
			{ segm.ptr<uchar>(j, i)[0] = 255;} }
}

void CVSystem::ScreentoneSegmentation::GetDistanceMap(std::vector<double> fv)
{
	for(int a = 0; a < img_width_scale; a++)
		for(int b = 0; b < img_height_scale; b++)
			{ gbrImg.ptr<float>(b, a)[0] = 1.0 / (1.0 +  GetDistance(fv, a, b)); }

	/*
	// DO NOT DELETE THIS
	if(SystemParams::segm_normalize)
	{
		std::cout << "Normalizing distance map\n";

		double dMax = DBL_MIN;
		double dMin = DBL_MAX;
		for(int a = 0; a < img_width_scale; a++)
		{
			for(int b = 0; b < img_height_scale; b++)
			{
				double val = gbrImg.ptr<float>(b, a)[0];
				if(val > dMax) dMax = val;
				if(val < dMin) dMin = val;
			}
		}

		std::cout << "GetDistanceMap() - Maximum distance map: " << dMax << "\n";
		std::cout << "GetDistanceMap() - Minimum distance map: " << dMin << "\n";

		// normalization
		for(int a = 0; a < img_width_scale; a++)
		{
			for(int b = 0; b < img_height_scale; b++)
			{		
				double d = (gbrImg.ptr<float>(b, a)[0] - dMin) / (dMax - dMin);
				gbrImg.ptr<float>(b, a)[0] = d;
			}
		}

		// Cut off
		for(int a = 0; a < img_width_scale; a++)
		{
			for(int b = 0; b < img_height_scale; b++)
			{		
				double d = gbrImg.ptr<float>(b, a)[0];
				if(d < SystemParams::segm_cut_off) d *= 0.1;
				gbrImg.ptr<float>(b, a)[0] = d;
			}
		}
	}
	*/
}

std::vector<double> CVSystem::ScreentoneSegmentation::GetStrokeFeature(std::vector<CVSystem::MyPoint> sPts)
{
	int numCluster = SystemParams::s_km_n_clusters;
	int maxIter = SystemParams::s_km_max_iter;

	std::cout << "Strokes' K-Means, number of points: " << sPts.size() << "\n";

	int* numMember = (int*)malloc(numCluster * sizeof(int));	// cluster size count
	int* indices = (int*)malloc(sPts.size() * sizeof(int));		// belongs to which cluster

	std::vector<std::vector<double>> centroids(numCluster);
	for(int a = 0; a < numCluster; a++)
		{ centroids[a] = std::vector<double>(SystemParams::g_total_dim); }
		
	// Get Features
	std::vector<std::vector<double> > fvectors;
	for(size_t a = 0; a < sPts.size(); a++)
	{
		fvectors.push_back(GetPixelFeature(sPts[a]));
		//indices[a] = -1;
		indices[a] = 0;
	}

	// Initial centroid (the firsts)
	for(int a = 0; a < numCluster; a++)
		for(int b = 0; b < SystemParams::g_total_dim; b++)
			{ centroids[a][b] = fvectors[a][b]; }

	bool isChanged = true;	// convergence flag
	int iter;
	for(iter = 0; iter < maxIter && isChanged; iter++)
	{	
		isChanged = false;
		for(size_t a = 0; a < sPts.size(); a++)	// per points
		{
			int closestIndex = 0;
			double smallestDistance = DBL_MAX;
			for(int b = 0; b < numCluster; b++)	// per centroid
			{
				double dist = GetDistance(fvectors[a], centroids[b]);
				if(dist < smallestDistance)
				{
					closestIndex = b;
					smallestDistance = dist;
				}
			}

			if(indices[a] != closestIndex)
			{
				isChanged = true;	// is it convergent ?
				indices[a] = closestIndex;
			}
		}

		// Reset centroids
		for(int a = 0; a < numCluster; a++)
		{
			for(int b = 0; b < SystemParams::g_total_dim; b++)
				centroids[a][b] = 0;
			numMember[a] = 0;	// member is equal to zero
		}

		// Recalculate centroids
		for(size_t a = 0; a < sPts.size(); a++)	// per feature
		{
			int index = indices[a];
			numMember[index]++;
			for(int b = 0; b < SystemParams::g_total_dim; b++)
				centroids[index][b] += fvectors[a][b];
		}

		// Normalize centroids
		for(int a = 0; a < numCluster; a++)
			for(int b = 0; b < SystemParams::g_total_dim; b++)
				centroids[a][b] /= numMember[a];

	}
	std::cout << "Strokes' K-Means, number of iterations: " << iter << "\n";

	// Obtain the major cluster
	int bIndex = -1;
	int maxNum = -1;
	for(int a = 0; a < numCluster; a++)
	{
		if(numMember[a] > maxNum)
		{
			bIndex = a;
			maxNum = numMember[a];
		}
	}

	std::cout << "Strokes' K-Means, biggest cluster size: " << maxNum << "\n";

	std::free(indices);
	std::free(numMember);	

	std::vector<double> fv(SystemParams::g_total_dim);
	for(int a = 0; a < SystemParams::g_total_dim; a++)
	{ fv[a] = centroids[bIndex][a]; }

	return fv;
}

std::vector<double> CVSystem::ScreentoneSegmentation::GetPixelFeature(CVSystem::MyPoint p)
{
	std::vector<double> feature( SystemParams::g_total_dim);
	for(int a = 0; a <  SystemParams::g_total_dim; a++)
		{ feature[a] = featureMatrix.ptr<float>(p.y, p.x)[a]; }
	return feature;
}

void CVSystem::ScreentoneSegmentation::Init(std::string filename, int width, int height)
{
	this->filename   = filename;
	this->img_ori_width  = width;
	this->img_ori_height = height;
	this->img_width_scale  = (int)(width *  SystemParams::s_rescale_factor);
	this->img_height_scale = (int)(height * SystemParams::s_rescale_factor);
	this->shouldCalc = true;

	this->inpImg = cv::imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
	this->oriInpImg = inpImg.clone();
	cv::Size sz(img_width_scale, img_height_scale);
	cv::resize(this->inpImg, this->inpImg, sz);

	gbrImg = cv::Mat::zeros(img_height_scale, img_width_scale, CV_32FC1);

	if(labelMap) delete labelMap;
	labelMap = (int*)malloc(width * height * sizeof(int));	
	for(int a = 0; a < width * height; a++)
		{ labelMap[a] = -1; }

	if(dilatedLabelMap) delete dilatedLabelMap;
	dilatedLabelMap = (int*)malloc(width * height * sizeof(int));	
	for(int a = 0; a < width * height; a++)
		{ dilatedLabelMap[a] = -1; }
}

void CVSystem::ScreentoneSegmentation::BuildFeature()
{
	// feature from file
	std::vector<std::string> fullpath = UtilityFunctions::split(this->filename, '//');
	std::vector<std::string> nameArray = UtilityFunctions::split(fullpath[fullpath.size() - 1], '.');
	std::string baseFile = "Feature/" + nameArray[0];	

	std::stringstream sstr;
	sstr << SystemParams::g_scale		<<  "_" 
		 << SystemParams::g_orientation <<  "_"
		 << SystemParams::g_window		<<  "_"
		 << SystemParams::g_ul			<<  "_" 
		 << SystemParams::g_uh			<<  "_"
		 << SystemParams::g_side		<<  "_" 
		 << SystemParams::g_flag		<<  "_"
		 << this->img_width_scale				<<  "_"
		 << this->img_height_scale;
	std::string numStr(sstr.str());

	std::string fullFilename = baseFile + "_" + numStr;
	
	bool featureExists = false;
	std::ifstream thefile(fullFilename); 
	if(thefile.good()) { featureExists = true; }
		
	if(!featureExists)
	{
		std::cout << "feature files don't exist, gabor wavelet calculation commenced...\n";
		gbr->Init();
		this->featureMatrix = gbr->GetFeatureMatrix(inpImg);

		// Writing file
		int dataSize = (SystemParams::g_scale * SystemParams::g_orientation  * 2 * this->img_width_scale  * this->img_height_scale);
		float* outData = (float *) calloc(dataSize, sizeof(float));
		if(outData)
		{
			int ctr = 0;
			for(int a = 0; a < this->img_width_scale; a++)
			{
				for(int b = 0; b < this->img_height_scale; b++)
				{
					for(int c = 0; c < SystemParams::g_total_dim; c++)
					{ 
						outData[ctr] = featureMatrix.ptr<float>(b, a)[c];
						ctr++;
					}
				}
			}
		}
		FILE* fp = fopen(fullFilename.c_str(), "wb");
		fwrite(outData, sizeof(float), dataSize, fp);
		fclose(fp);
		free(outData);
	}
	else
	{
		std::cout << "feature file exists, reading file...\n";
		this->featureMatrix = cv::Mat::zeros(this->img_height_scale, this->img_width_scale, CV_GDIM);

		FILE* fp;
		if((fp = fopen(fullFilename.c_str(), "rb")) != NULL)
		{
			int dataSize = (SystemParams::g_scale * SystemParams::g_orientation  * 2 * this->img_width_scale  * this->img_height_scale);
			float* inData = (float *) calloc(dataSize, sizeof(float));
			if(inData)
			{
				int ctr = 0;
				fread(inData, sizeof(float), dataSize, fp);
				fclose(fp);
				for(int a = 0; a < this->img_width_scale; a++)
				{
					for(int b = 0; b < this->img_height_scale; b++)
					{
						for(int c = 0; c < SystemParams::g_total_dim; c++)
						{ 
							featureMatrix.ptr<float>(b, a)[c] = inData[ctr];
							ctr++;
						}
					}
				}
			}
		}
	}

	// Calculate standard deviations 
	// Currently for debug only
	stds.clear();
	FeatureStdDev(featureMatrix, stds);

	//for(int a = 0; a < SystemParams::g_total_dim; a++)
	//	std::cout << stds[a] << "\n";

	this->shouldCalc = false;
}

double CVSystem::ScreentoneSegmentation::GetDistance(std::vector<double> f1, std::vector<double> f2)
{
	double dist = 0.0;
	for(int a = 0; a < SystemParams::g_total_dim; a++)
	{
		double d = f1[a] - f2[a];
		dist += d * d;
	}
	return dist;
}

double CVSystem::ScreentoneSegmentation::GetDistance(std::vector<double> fv, int x, int y)
{
	double dist = 0.0;
	for(int a = 0; a < SystemParams::g_total_dim; a++)
	{
		double d = fv[a] - featureMatrix.ptr<float>(y, x)[a];
		dist += d * d;
	}
	return dist;
}

// http://www.johndcook.com/standard_deviation.html
void CVSystem::ScreentoneSegmentation::FeatureStdDev(cv::Mat fv, std::vector<double>& stds)
{
	stds.clear();
	stds = std::vector<double>(SystemParams::g_total_dim );
	
	for(int c = 0; c < SystemParams::g_total_dim; c++)
	{
		RunningStat rStat;
		
		for(int a = 0; a < fv.cols; a++)
			for(int b = 0; b < fv.rows; b++)
				{ rStat.Push(fv.ptr<float>(b, a)[c]); }

		stds[c] = rStat.StandardDeviation();
	}
}

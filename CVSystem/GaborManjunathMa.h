
/*
 *
 * Gabor wavelet calculation
 *
 * Manjunath, B. S. & Ma, W. 
 * Texture features for browsing and retrieval of image data 
 * IEEE Transactions on Pattern Analysis and Machine Intelligence (PAMI - Special issue on Digital Libraries), 1996, 18, 837-42
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 * 
 */

#ifndef __GABOR_WAVELET__
#define __GABOR_WAVELET__

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "GaborMatrix.h"
#include "RunningStat.h"

class GaborManjunathMa
{
public:
	// Constructor
	GaborManjunathMa();

	// Destructor
	~GaborManjunathMa();

	// Init parameters and matrices
	void Init();

	// Get feature matrix
	cv::Mat GetFeatureMatrix(cv::Mat imgCV);
	
private:
	// real matrix
	MatrixStruct** rMatrices;

	// imaginary matrix
	MatrixStruct** iMatrices;

	// parameters
	int side;
	int scale;
	int orientation;
	int flag;
	double Ul;
	double Uh;
	int windowSize;
	
	// statistics
	RunningStat* rStat;

private:
	// Create gabor matrix
	void CreateGabor(MatrixStruct *Gr, MatrixStruct *Gi, int s, int n, double Ul, double Uh, int scale, int orientation, int flag);

	// calculate gabor wavelet
	void GaborFilteredImg(MatrixStruct *FilteredImg_real,	MatrixStruct *FilteredImg_imag, MatrixStruct *input_img, MatrixStruct** rMatrices, MatrixStruct** iMatrices, int side, double Ul, double Uh, int num_scale, int num_orientation, int flag);
	
	// get mean and standard deviation
	void MeanStddev(cv::Mat F_complex, double& mean, double& stddev, int img_height, int img_width, int win_size, int cur_scale, int cur_orient, int px, int py);
};

#endif
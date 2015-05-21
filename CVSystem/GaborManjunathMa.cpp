
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "GaborManjunathMa.h"
#include "SystemParams.h"



GaborManjunathMa::GaborManjunathMa() : rStat(0)
{
}

GaborManjunathMa::~GaborManjunathMa()
{
	if(rStat) delete rStat;
}

cv::Mat GaborManjunathMa::GetFeatureMatrix(cv::Mat imgCV)
{
	cv::Mat fMatrix = cv::Mat::zeros(imgCV.rows, imgCV.cols, CV_GDIM);

	cv::Mat grayImg;
	imgCV.convertTo(grayImg, CV_32FC1);
			
	int pad = this->windowSize / 2;
	cv::copyMakeBorder( grayImg, grayImg, pad, pad, pad, pad, cv::BORDER_REFLECT_101);
	
	MatrixStruct* imgG;
	cv::normalize(grayImg, grayImg, 0.0, 1.0, cv::NORM_MINMAX, CV_32FC1);
	MatrixOperations::CreateMatrix(&imgG, grayImg.rows, grayImg.cols);
	for(int a = 0; a < grayImg.cols; a++)
		for(int b = 0; b < grayImg.rows; b++)
			imgG->data[b][a] = grayImg.ptr<float>(b, a)[0];

	int res_height = grayImg.rows * scale;
	int res_width  = grayImg.cols * orientation;

	MatrixStruct* F_r;
	MatrixStruct* F_i;
	MatrixOperations::CreateMatrix(&F_r, res_height, res_width);
	MatrixOperations::CreateMatrix(&F_i, res_height, res_width);

	GaborFilteredImg(F_r, F_i, 
					 imgG, 
					 this->rMatrices, this->iMatrices, 
					 this->side, 
					 this->Ul,  this->Uh, 
					 this->scale, this->orientation, 
					 this->flag);

	// Magnitude of complex value (double precision)
	cv::Mat magImg = cv::Mat::zeros(res_height, res_width, CV_32FC1);
	for (int h = 0; h < res_height; h++)
		for (int w = 0; w < res_width; w++)
			magImg.ptr<float>(h, w)[0] = sqrt(F_r->data[h][w] * F_r->data[h][w] + F_i->data[h][w] * F_i->data[h][w]);
	
	// Debug
	cv::Mat magDebugImg;
	cv::normalize(magImg, magDebugImg, 0.0, 255.0, cv::NORM_MINMAX, CV_32FC1);
	cv::imwrite("image_magnitude.png", magDebugImg);

	for(int s = 0; s < scale; s++)
	{
		for(int o = 0; o < orientation; o++)
		{
			//int idx = (s + o * scale) * 2;
			int idx = (s + o * scale);
			for(int x = 0; x < fMatrix.cols; x++)
			{
				for(int y = 0; y < fMatrix.rows; y++)
				{
					double mean = 0.0;
					double stddev = 0.0;

					if(this->windowSize > 1) MeanStddev(magImg, mean, stddev, grayImg.rows, grayImg.cols, windowSize, s, o, x + pad, y + pad);
					else			   mean = magImg.ptr<float>(grayImg.rows * s + y, grayImg.cols * o + x)[0];

					// I don't include standard deviation
					//fMatrix.ptr<float>(y, x)[idx] =     stddev;
					fMatrix.ptr<float>(y, x)[idx] =     mean;
				}
			}
		}
	}

	MatrixOperations::FreeMatrix(imgG);
	MatrixOperations::FreeMatrix(F_r);
	MatrixOperations::FreeMatrix(F_i);

	return fMatrix;
}

void GaborManjunathMa::MeanStddev(cv::Mat magImg, double& mean, double& stddev, int img_height, int img_width, int win_size, int cur_scale, int cur_orient, int px, int py)
{
	RunningStat rrStat;
	int xw = px - (win_size / 2);
	int yw = py - (win_size / 2);

	for(int a = xw; a < xw + win_size; a++)
		for(int b = yw; b < yw + win_size; b++)
			rrStat.Push(magImg.ptr<float>(img_height * cur_scale + b, img_width * cur_orient + a)[0]);
	
	mean = rrStat.Mean();
	stddev = rrStat.StandardDeviation();
}

void GaborManjunathMa::Init()
{
	rStat = new RunningStat();

	this->scale		  = SystemParams::g_scale;
	this->orientation = SystemParams::g_orientation;
	this->windowSize  = SystemParams::g_window;	
	this->Ul		  = SystemParams::g_ul;
	this->Uh		  = SystemParams::g_uh;
	this->side        = SystemParams::g_side;
	this->flag        = SystemParams::g_flag;

	std::cout << "Initializing Gabor Kernels: " << this->scale << " "
												<< this->orientation << " "
												<< this->windowSize << " "
												<< this->Ul << " "
												<< this->Uh << " "
												<< this->side << " "
												<< this->flag << "\n";

	int filter_width = 2 * side + 1;
	int filter_height = 2 * side + 1;

	rMatrices = (MatrixStruct**)malloc(scale * orientation * sizeof(MatrixStruct*));
	iMatrices = (MatrixStruct**)malloc(scale * orientation * sizeof(MatrixStruct*));

	for(int i = 0; i < scale; i++)
	{
		for(int j = 0;j < orientation; j++)
		{
			MatrixOperations::CreateMatrix(&rMatrices[i * orientation + j], filter_height, filter_width);
			MatrixOperations::CreateMatrix(&iMatrices[i * orientation + j], filter_height, filter_width);
			CreateGabor(rMatrices[i * orientation + j], iMatrices[i * orientation + j], i + 1, j + 1, Ul, Uh, scale, orientation, flag);
		}
	}
}

// http://www.svcl.ucsd.edu/~nikux/test/html/gabor_8c-source.html
void GaborManjunathMa::CreateGabor(MatrixStruct *Gr, MatrixStruct *Gi, int s, int n, double Ul, double Uh, int scale, int orientation, int flag)
{
	double base = Uh / Ul;
	double a = pow(base, 1.0/(double)(scale-1));
	double u0 = Uh / pow(a, (double) scale-s);

	double Uvar = (a-1.0)*u0/((a+1.0)*sqrt(2.0*log(2.0)));

	double z = -2.0*log(2.0)*(Uvar*Uvar)/u0;
	double Vvar = tan(M_PI/(2.0*(double)orientation))*(u0+z)/sqrt(2.0*log(2.0)-z*z/(Uvar*Uvar));

	double Xvar = 1.0 / (2.0 * M_PI * Uvar);
	double Yvar = 1.0 / (2.0 * M_PI * Vvar);

	double t1 = cos(M_PI / (double)orientation * ((double)n - 1.0));
	double t2 = sin(M_PI / (double)orientation * ((double)n - 1.0));

	int side = (int) (Gr->h - 1) / 2;

	for (int x = 0; x < 2 * side + 1; x++) 
	{
		for (int y = 0; y < 2 * side + 1; y++) 
		{
			double X = (double)  (x - side) * t1 + (double) (y - side) * t2;
			double Y = (double) -(x - side) * t2 + (double) (y - side) * t1;
			double G = 1.0 / (2.0 * M_PI * Xvar * Yvar) * pow(a, (double) scale - s) * 
					   exp(-0.5 * ((X * X) / (Xvar * Xvar) + (Y * Y) / (Yvar * Yvar)));
			Gr->data[x][y] = G * cos(2.0 * M_PI * u0 * X);
			Gi->data[x][y] = G * sin(2.0 * M_PI * u0 * X);
		}
	}

	/* if flag = 1, then remove the DC from the filter */
	if (flag == 1) 
	{
		double m = 0;
		for (int x = 0; x < 2 * side + 1; x++)
			for (int y = 0; y < 2 * side + 1; y++)
				m += Gr->data[x][y];

		m /= pow((double) 2.0 * side + 1, 2.0);

		for (int x = 0; x < 2 * side + 1; x++)
			for (int y = 0; y < 2 * side + 1; y++)
				Gr->data[x][y] -= m;
	}	
}

// http://www.svcl.ucsd.edu/~nikux/test/html/gabor_8c-source.html
void GaborManjunathMa::GaborFilteredImg(MatrixStruct *FilteredImg_real, MatrixStruct *FilteredImg_imag, MatrixStruct *input_img, MatrixStruct** rMatrices, MatrixStruct** iMatrices, int side, double Ul, double Uh, int num_scale, int num_orientation, int flag)
{
	int r1; int r2; int r3; int r4;

	MatrixStruct* IMG_real;	// power of two dimension of input image
	MatrixStruct* IMG_imag;	// power of two
	MatrixStruct* Tmp_1;
	MatrixStruct* Tmp_2;
	MatrixStruct* F_1;
	MatrixStruct* F_2;
	MatrixStruct* G_real;
	MatrixStruct* G_imag;
	MatrixStruct* F_IMG_real;
	MatrixStruct* F_IMG_imag;

	int img_height = input_img->h;
	int img_width = input_img->w;

	// FFT2 (the dimension of image should be power of two)	
	int xs = (int) pow(2.0, ceil(MatrixOperations::log2((double)(input_img->w + 2.0 * side))));
	int ys = (int) pow(2.0, ceil(MatrixOperations::log2((double)(input_img->h + 2.0 * side))));

	// creating matrix with height of xs and ys (square power-of-two)
	MatrixOperations::CreateMatrix(&IMG_real, ys, xs);

	// copying image to power-of-two matrix
	r1 = input_img->w + side; r2 = input_img->w + 2 * side;
	for (int h = 0; h < side; h++) 
	{
		for (int w = 0; w < side; w++)
			IMG_real->data[h][w] = input_img->data[side-1-h][side-1-w];

		for (int w = side; w < r1; w++)
			IMG_real->data[h][w] = input_img->data[side-1-h][w-side];

		for (int w = r1; w < r2; w++)
			IMG_real->data[h][w] = input_img->data[side - 1 - h][2 * input_img->w - w + side - 1];
	}

	r1 = input_img->h+side; r2 = input_img->w+side; r3 = input_img->w+2*side;
	for (int h=side; h < r1; h++) 
	{
		for (int w = 0; w < side; w++)
			IMG_real->data[h][w] = input_img->data[h-side][side-1-w];

		for (int w = side; w < r2; w++)
			IMG_real->data[h][w] = input_img->data[h-side][w-side];

		for (int w = r2; w < r3; w++)
			IMG_real->data[h][w] = input_img->data[h-side][2*input_img->w-w+side-1];
	}

	r1 = input_img->h+side; r2 = input_img->h+2*side;
	r3 = input_img->w+side; r4 = input_img->w+2*side;
	for (int h = r1; h < r2; h++) 
	{
		for (int w = 0; w < side; w++)
			IMG_real->data[h][w] = input_img->data[2*input_img->h-h+side-1][side-1-w];

		for (int w = side; w < r3; w++)
			IMG_real->data[h][w] = input_img->data[2*input_img->h-h+side-1][w-side];

		for (int w = r3; w < r4; w++)
			IMG_real->data[h][w] = input_img->data[2*input_img->h-h+side-1][2*input_img->w-w+side-1];
	}

	MatrixOperations::CreateMatrix(&F_IMG_real, ys, xs);
	MatrixOperations::CreateMatrix(&F_IMG_imag, ys, xs);
	MatrixOperations::CreateMatrix(&IMG_imag,   ys, xs);

	MatrixOperations::Mat_FFT2(F_IMG_real, F_IMG_imag,	
							   IMG_real,		// The matrix containing the real part of input
							   IMG_imag);		// The matrix containing the imaginary part of input

	/* ----------- compute the Gabor filtered output ------------- */

	MatrixOperations::CreateMatrix(&Tmp_1,  ys, xs);
	MatrixOperations::CreateMatrix(&Tmp_2,  ys, xs);
	MatrixOperations::CreateMatrix(&F_1,	ys, xs);
	MatrixOperations::CreateMatrix(&F_2,	ys, xs);
	MatrixOperations::CreateMatrix(&G_real, ys, xs);
	MatrixOperations::CreateMatrix(&G_imag, ys, xs);

	for (int s = 0; s < num_scale; s++) 
	{
		for (int n = 0; n < num_orientation; n++) 
		{
			// Resizing the gabor filters to have size of [xs, ys]
			//MatrixOperations::Mat_Copy(F_1, rMatrices[s * num_orientation + n], 0, 0, 0, 0, 2 * side, 2 * side);
			//MatrixOperations::Mat_Copy(F_2, iMatrices[s * num_orientation + n], 0, 0, 0, 0, 2 * side, 2 * side);
			
			// This one is faster copy but I'm not sure it's correct (I'm maybe too paranoid)
			MatrixOperations::Mat_MemCopy(F_1, rMatrices[s * num_orientation + n], 2 * side, 2 * side);
			MatrixOperations::Mat_MemCopy(F_2, iMatrices[s * num_orientation + n], 2 * side, 2 * side);

			MatrixOperations::Mat_FFT2(G_real, G_imag, F_1, F_2);

			MatrixOperations::GetRealPart(IMG_real, G_real, G_imag, F_IMG_real, F_IMG_imag);
			MatrixOperations::GetImaginaryPart(IMG_imag, G_real, G_imag, F_IMG_real, F_IMG_imag);

			MatrixOperations::Mat_IFFT2(Tmp_1, Tmp_2, IMG_real, IMG_imag);

			// will be copied per scale and orientation
			MatrixOperations::Mat_Copy(FilteredImg_real,				// target
									   Tmp_1,							// source
									   s * img_height,					// h start index of target
									   n * img_width,					// w start index of target
									   2 * side,						// h start index of source
									   2 * side,						// w start index of source
									   img_height + 2 * side - 1,		// h end index of source
									   img_width + 2 * side - 1);		// w end index of source

			MatrixOperations::Mat_Copy(FilteredImg_imag, 
									   Tmp_2, 
									   s * img_height, 
									   n * img_width, 
									   2 * side, 
									   2 * side, 
									   img_height + 2 * side - 1, 
									   img_width + 2 * side - 1);
		}
	}

	MatrixOperations::FreeMatrix(Tmp_1);
	MatrixOperations::FreeMatrix(Tmp_2);
	MatrixOperations::FreeMatrix(F_1);
	MatrixOperations::FreeMatrix(F_2);
	MatrixOperations::FreeMatrix(G_real);
	MatrixOperations::FreeMatrix(G_imag);
	MatrixOperations::FreeMatrix(F_IMG_real);
	MatrixOperations::FreeMatrix(F_IMG_imag);
	MatrixOperations::FreeMatrix(IMG_real);
	MatrixOperations::FreeMatrix(IMG_imag);
}





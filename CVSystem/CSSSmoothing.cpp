
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "CSSSmoothing.h"

double round(double number) { return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5); }

CVSystem::CSSSmoothing::CSSSmoothing()
{
	this->Init2();
}

CVSystem::CSSSmoothing::~CSSSmoothing()
{
	
}

void CVSystem::CSSSmoothing::Init1()
{
	this->_sigma = SystemParams::css_sigma;

	this->_M = round((10.0*this->_sigma+1.0) / 2.0) * 2 - 1;
	assert(this->_M % 2 == 1); // M is an odd number

	getGaussianDerivs(this->_sigma, this->_M, this->_g, this->_dg, this->_d2g);
}

void CVSystem::CSSSmoothing::Init2()
{
	this->_sigma = SystemParams::css_sigma;
	this->_min_affinity = 5;

	this->_M = round((10.0*this->_sigma+1.0) / 2.0) * 2 - 1;
	assert(this->_M % 2 == 1); // M is an odd number

	getGaussian(this->_sigma, this->_M, this->_g);
}

// Original CSS Smoothing (Also calculates first and second derivatives)
void CVSystem::CSSSmoothing::SmoothCurve1(std::vector<CVSystem::MyPoint>& oriCurve)
{
	if(this->_sigma <= 0) { return; }

	//std::cout << "CSS SMoothing..." << oriCurve.size() << "\n";
	if(oriCurve.size() < (this->_M - 1) / 2) { /*std::cout << "Too few points!\n";*/ return; }
			
	std::vector<double> curvex, curvey, smoothx, smoothy;
	PolyLineSplit(oriCurve,curvex,curvey);

	std::vector<double> X,XX,Y,YY;
	getdXcurve(curvex, this->_sigma, smoothx, X, XX, this->_g, this->_dg, this->_d2g, false);
	getdXcurve(curvey, this->_sigma, smoothy, Y, YY, this->_g, this->_dg, this->_d2g, false);

	PolyLineMerge(oriCurve, smoothx, smoothy);
}

// Speedup version (No derivatives calculation)
void CVSystem::CSSSmoothing::SmoothCurve2(std::vector<CVSystem::MyPoint>& oriCurve, cv::Mat distance_img)
{
	if(this->_sigma <= 0) { return; }

	//std::cout << "CSS SMoothing..." << oriCurve.size() << "\n";
	if(oriCurve.size() < (this->_M - 1) / 2) { /*std::cout << "Too few points!\n";*/ return; }
			
	std::vector<CVSystem::MyPoint> smoothCurve;

	getdXYcurve(oriCurve, this->_sigma, smoothCurve, this->_g, distance_img);

	for(size_t a = 0; a < oriCurve.size(); a++)
	{
		oriCurve[a].x = smoothCurve[a].x;
		oriCurve[a].y = smoothCurve[a].y;
	}
}

// Gaussian kernel only
void CVSystem::CSSSmoothing::getGaussian(double sigma, int M, std::vector<double>& gaussian)
{
	int L = (M - 1) / 2;
	double sigma_sq = sigma * sigma;
	double sigma_quad = sigma_sq*sigma_sq;
	gaussian.resize(M);

	cv::Mat_<double> g = cv::getGaussianKernel(M, sigma, CV_64F);
	for (double i = -L; i < L+1.0; i += 1.0) 
	{
		int idx = (int)(i+L);
		gaussian[idx] = g(idx);
	}
}

void CVSystem::CSSSmoothing::getGaussianDerivs(double sigma, int M, std::vector<double>& gaussian, std::vector<double>& dg, std::vector<double>& d2g) 
{
	int L = (M - 1) / 2;
	double sigma_sq = sigma * sigma;
	double sigma_quad = sigma_sq*sigma_sq;
	dg.resize(M); 
	d2g.resize(M); 
	gaussian.resize(M);

	cv::Mat_<double> g = cv::getGaussianKernel(M, sigma, CV_64F);
	for (double i = -L; i < L+1.0; i += 1.0) 
	{
		int idx = (int)(i+L);
		gaussian[idx] = g(idx);
		// from http://www.cedar.buffalo.edu/~srihari/CSE555/Normal2.pdf
		dg[idx] = (-i/sigma_sq) * g(idx);
		d2g[idx] = (-sigma_sq + i*i)/sigma_quad * g(idx);
	}
}

void CVSystem::CSSSmoothing::getdXYcurve(std::vector<CVSystem::MyPoint> oriCurve, 
										 double sigma, 
										 std::vector<CVSystem::MyPoint>& smoothCurve, 
										 std::vector<double> gaussianKernel,
										 cv::Mat distance_img)
{
	smoothCurve.resize(oriCurve.size());
	for(size_t a = 0; a < smoothCurve.size(); a++)
	{
		smoothCurve[a].x = oriCurve[a].x;
		smoothCurve[a].y = oriCurve[a].y;
	}

	for (size_t i = 0; i < oriCurve.size(); i++) 
	{
		//std::cout << oriCurve[i].x << " " << oriCurve[i].y << "\n";
		//if(distance_img.ptr<float>(oriCurve[i].y, oriCurve[i].x)[0] <= this->_min_affinity)

		// These code below is really ugly...
		int dist = (int)distance_img.ptr<float>(oriCurve[i].y, oriCurve[i].x)[0];
		bool smoothFlag = (dist <= this->_min_affinity);
		if(!smoothFlag && dist < (this->_min_affinity + 10)) smoothFlag = ShouldDoSmoothing(oriCurve, i, distance_img);

		if(smoothFlag)
		{
			CVSystem::MyPoint smoothedPoint; 
			getdXY(oriCurve,i,sigma,smoothedPoint, gaussianKernel);
			smoothCurve[i] = smoothedPoint;
		}
	}
}

bool CVSystem::CSSSmoothing::ShouldDoSmoothing(std::vector<CVSystem::MyPoint> oriCurve, int n, cv::Mat distance_img)
{
	cv::vector<int> distList;

	int L = 2;
	for (int k = -L; k < L + 1; k++) 
	{
		int n_k = n - k;
		int idx = n_k;
		if (n_k < 0)  { idx = oriCurve.size() + n_k; } 
		else if(n_k > oriCurve.size() - 1)  { idx = n_k - (oriCurve.size()); } 

		int x = oriCurve[idx].x;
		int y = oriCurve[idx].y;

		distList.push_back((int)distance_img.ptr<float>(y, x)[0]);
	}

	std::sort(distList.begin(), distList.end());
	int dist = distList[distList.size() / 2];

	if(dist < this->_min_affinity)
		return true;

	return false;
}

void CVSystem::CSSSmoothing::getdXY(std::vector<CVSystem::MyPoint> oriCurve, 
									int n, 
									double sigma, 
									CVSystem::MyPoint& smoothPoint, 
									std::vector<double> gaussianKernel)
{
	int L = (gaussianKernel.size() - 1) / 2;

	smoothPoint.x = 0.0;
	smoothPoint.y = 0.0;
	//  cout << "Point " << n << ": ";
	for (int k = -L; k < L + 1; k++) 
	{
		double x_n_k;
		double y_n_k;
		int n_k = n - k;

		int idx = n_k;

		if (n_k < 0)  { idx = oriCurve.size() + n_k; } 
		else if(n_k > oriCurve.size() - 1)  { idx = n_k - (oriCurve.size()); } 

		x_n_k = oriCurve[idx].x;
		y_n_k = oriCurve[idx].y;

		smoothPoint.x += x_n_k * gaussianKernel[k + L];
		smoothPoint.y += y_n_k * gaussianKernel[k + L];
	}
}

/* 1st and 2nd derivative of smoothed curve point */
void  CVSystem::CSSSmoothing::getdX(std::vector<double> x, 
									int n, 
									double sigma, 
									double& gx, 
									double& dgx, 
									double& d2gx, 
									std::vector<double> g, 
									std:: vector<double> dg, 
									std::vector<double> d2g,
									bool isOpen) 
{       
	int L = (g.size() - 1) / 2;

	gx = dgx = d2gx = 0.0;
	//  cout << "Point " << n << ": ";
	for (int k = -L; k < L + 1; k++) 
	{
		double x_n_k;
		int n_k = n - k;

		if (n_k < 0) 
		{
			if (isOpen) 
			{
				//open curve - mirror values on border
				x_n_k = x[-n_k]; 
			} 
			else 
			{
				//closed curve - take values from end of curve
				x_n_k = x[x.size() + n_k];
			}           
		} 
		else if(n_k > x.size()-1) 
		{
			if (isOpen) 
			{
				//mirror value on border
				x_n_k = x[n+k]; 
			} 
			else 
			{
				x_n_k = x[n_k-(x.size())];
			}           
		} 
		else 
		{
			//          cout << n-k;
			x_n_k = x[n_k];
		}
		//      cout << "* g[" << g[k + L] << "], ";

		gx   += x_n_k * g[k + L]; //gaussians go [0 -> M-1]
		dgx  += x_n_k * dg[k + L]; 
		d2gx += x_n_k * d2g[k + L];
	}
	//  cout << endl;
}



/* 0th, 1st and 2nd derivatives of whole smoothed curve */
void  CVSystem::CSSSmoothing::getdXcurve(std::vector<double> x, 
										 double sigma, 
										 std::vector<double>& gx, 
										 std::vector<double>& dx, 
										 std::vector<double>& d2x, 
										 std::vector<double> g, 
										 std::vector<double> dg, 
										 std::vector<double> d2g,
										 bool isOpen) 
{   
	gx.resize(x.size()); 
	dx.resize(x.size()); 
	d2x.resize(x.size());
	for (size_t i=0; i<x.size(); i++) 
	{
		double gausx,dgx,d2gx; 
		getdX(x,i,sigma,gausx,dgx,d2gx,g,dg,d2g,isOpen);
		gx[i] = gausx;
		dx[i] = dgx;
		d2x[i] = d2gx;
	}
}


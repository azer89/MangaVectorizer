/*
 * Copyright (c) 2009-2011, A. Buades <toni.buades@uib.es>
 * All rights reserved.
 * 
 * License:
 *
 * This program is provided for scientific and educational only:
 * you can use and/or modify it for these purposes, but you are
 * not allowed to redistribute this work or derivative works in
 * source or executable form. A license must be obtained from the
 * patent right holders for any other use.
 *
 *
 */

/**
 * @file   LibCartoon.cpp
 * @brief  Cartoon + texture functions
 * @author Antoni Buades <toni.buades@uib.es>
 */
#include "stdafx.h"

#include <string.h>
#include "LibCartoon.h"

/**
 * \brief   Cartoon + texture decomposition for a single channel image 
 *
 * @param[in]   input			input image 
 * @param[out]  out             output vector
 * @param[in]   sigma			standard deviation of the low pass filter
 * @param[in]   width, height	size of the image
 */
void non_linear_cartoon(float * input, float * out, float sigma,  int width, int height)
{
	int size = width*height;        

	// kernel <- G_sigma  :  1D Gaussian kernel of standard 
	//                       deviation equal sigma
	int ksize;
	float *kernel = fiFloatGaussKernel(sigma,ksize);


	// auxiliary variable for gradient computation
	float *grad = (float *) malloc(size*sizeof(float));

	// ratio1 <- G_sigma * |Df|
	float *ratio1 = (float *) malloc(size*sizeof(float));
	fiComputeImageGradient(input,grad,NULL,width,height);
	fiSepConvol(grad,ratio1,width,height, kernel, ksize,kernel,ksize);

	//  ratio2 <- G*|D(L_sigma*f)| 
	//  being L_sigma a low pass filter depending on sigma and niter
	float *gconvolved = (float *) malloc(size*sizeof(float));

	int niter = 5;          
	low_pass_filter(input,gconvolved,sigma,niter,width,height);

	float *ratio2 = (float *) malloc(size*sizeof(float));
	fiComputeImageGradient(gconvolved,grad,NULL,width,height);
	fiSepConvol(grad,ratio2,width,height, kernel, ksize,kernel,ksize);

	// for each pixel compute the weighted average  of  G_sigma * |Df| 
	// and G*|D(L_sigma*f)| 
	for(int i=0; i < size; i++)
	{
		float weight = WeightingFunction(ratio1[i] ,ratio2[i]);
		out[i] = weight * gconvolved[i] + (1.0 - weight) * input[i];
	}                       
	
	free(ratio1);
	free(ratio2);
	free(grad);
	free(kernel);   

	free(gconvolved);
}

/**
 * \brief   Cartoon + texture decomposition for a color image 
 *
 * @param[in]   ired, igreen, iblue             input image 
 * @param[out]  ored, ogreen, oblue             output vector
 * @param[in]   sigma                           standard deviation of the low
 *                                              pass filter
 * @param[in]   width, height                   size of the image
 *
 */
void non_linear_cartoon(float *ired, float *igreen, float *iblue, float *ored,
                        float *ogreen, float *oblue, float sigma, int width, 
                        int height)
{
	int size = width*height;

	// kernel <- G_sigma  :  1D Gaussian kernel of standard deviation 
	//                       equal sigma
	int ksize;
	float *kernel = fiFloatGaussKernel(sigma,ksize);


	// auxiliary variable for gradient computation
	float *grad = (float *) malloc(size*sizeof(float));

	float *ratioaux = (float *) malloc(size*sizeof(float));
	for(int i= 0; i < size; i++) ratioaux[i] = 0;        

	// ratio1 <- (G_sigma * |Df_red| + G_sigma * |Df_green| + 
	//            G_sigma * |Df_blue|) / 3.0; 
	float *ratio1 = (float *) malloc(size*sizeof(float));

	fiComputeImageGradient(ired,grad,NULL,width,height);
	fiSepConvol(grad,ratioaux,width,height, kernel, ksize,kernel,ksize);
	for(int i= 0; i < size; i++) ratio1[i] = ratioaux[i];

	fiComputeImageGradient(igreen,grad,NULL,width,height);
	fiSepConvol(grad,ratioaux,width,height, kernel, ksize,kernel,ksize);
	for(int i= 0; i < size; i++) ratio1[i] += ratioaux[i];

	fiComputeImageGradient(iblue,grad,NULL,width,height);
	fiSepConvol(grad,ratioaux,width,height, kernel, ksize,kernel,ksize);
	for(int i= 0; i < size; i++) ratio1[i] += ratioaux[i];
	for(int i= 0; i < size; i++) ratio1[i] /= 3.0f;

	//  ratio2 <- (G*|D(L_sigma*f_r)| +  G*|D(L_sigma*f_r)| +  
	//             G*|D(L_sigma*f_r)|) / 3.0;
	//  being L_sigma a low pass filter depending on sigma and niter

	int niter=5;
	float *ratio2 = (float *) malloc(size*sizeof(float));
	float *rconvolved = (float *) malloc(size*sizeof(float));
	float *gconvolved = (float *) malloc(size*sizeof(float));
	float *bconvolved = (float *) malloc(size*sizeof(float));

	low_pass_filter(ired,rconvolved,sigma,niter,width,height);
	low_pass_filter(igreen,gconvolved,sigma,niter,width,height);
	low_pass_filter(iblue,bconvolved,sigma,niter,width,height);

	fiComputeImageGradient(rconvolved,grad,NULL,width,height);
	fiSepConvol(grad,ratioaux,width,height, kernel, ksize,kernel,ksize);
	for(int i= 0; i < size; i++) ratio2[i] = ratioaux[i];

	fiComputeImageGradient(gconvolved,grad,NULL,width,height);
	fiSepConvol(grad,ratioaux,width,height, kernel, ksize,kernel,ksize);
	for(int i= 0; i < size; i++) ratio2[i] += ratioaux[i];

	fiComputeImageGradient(bconvolved,grad,NULL,width,height);
	fiSepConvol(grad,ratioaux,width,height, kernel, ksize,kernel,ksize);
	for(int i= 0; i < size; i++) ratio2[i] += ratioaux[i];
	for(int i= 0; i < size; i++) ratio2[i] /= 3.0f;


	// for each pixel compute the weighted average  of  G_sigma * |Df| 
	// and G*|D(L_sigma*f)| 
	for(int i=0; i < size; i++)
	{   
		float weight = WeightingFunction(ratio1[i],ratio2[i]);

		ored[i] = weight * rconvolved[i] + (1.0 - weight) * ired[i];
		ogreen[i] = weight * gconvolved[i] + (1.0 - weight) * igreen[i];
		oblue[i] = weight * bconvolved[i] + (1.0 - weight) * iblue[i];
	} 

	free(ratio1);
	free(ratio2);
	free(ratioaux);

	free(grad);
	free(kernel);   

	free(rconvolved);
	free(gconvolved);
	free(bconvolved);
}

/**
 * \brief   Low Pass filter L_sigma f 
 *  The low pass filter is approximated by   L = Id - (Id - G_sigma)^{n *}
 *  That is, the difference between f and G_sigma(f) is convolved again with
 *  G_sigma, and the result of this convolution is put back to f
 *  This approach is iterated niter times
 *
 * @param[in]   input			input image 
 * @param[out]  out             output vector
 * @param[in]   sigma			standard deviation of the low pass filter
 * @param[in]   niter			number of iterations
 * @param[in]   width, height   size of the image
 */
void low_pass_filter(float * input, float * out, float sigma, int niter,  int width, int height)
{
	int size = width*height;

	// build a 1D gaussian kernel of std sigma:  kernel <- G_sigma
	int ksize;
	float *kernel = fiFloatGaussKernel(sigma,ksize);

	// memory for G_sigma * f   and   f - G_sigma * f 
	float *imconvolved = (float *) malloc(size*sizeof(float));
	float *imdifference = (float *) malloc(size*sizeof(float));

	// out  <-  G_sigma * f
	fiSepConvol(input,out,width,height,kernel,ksize,kernel,ksize);

	if (niter > 0)
	{
		// imdifference <-  input - out
		fpCombine(input,1.0,out,-1.0,imdifference,width*height);  

		for(int i=0;i<niter;i++)
		{

			// imconvolved <-  G_sigma * imdifference
			fiSepConvol(imdifference,imconvolved,width,height, kernel, ksize,kernel,ksize); 

			// imdifference <- imdifference - G_sigma * imdifference
			fpCombine(imdifference,1.0,imconvolved,-1.0, imdifference,width*height);
		}
		// out <- input - imdifference
		fpCombine(input,1.0,imdifference,-1.0,out,size);
	}
	free(kernel);   
	if (imdifference) free(imdifference);
	if (imconvolved) free(imconvolved);
}

/**
 * \brief  Weighting function w(lambda(x)) being lambda(x) = (r1 - r2) / r1 
 *         and w(x) the linear ramp going from 0 to 1 between  lambda=0.25 and
 *         lambda=0.5
 *
 * @param[in]   r1        G_sigma * |Df|        
 * @param[in]   r2        G*|D(L_sigma*f)|
 */
float WeightingFunction(float r1, float r2)
{
	float difference = r1 - r2;

	float ar1 = (float) fabs((double) r1);
	if (ar1 > 1.0) difference /= ar1;
	else difference = 0.0;


	float weight;
	float cmin= 0.25f;
	float cmax= 0.50f; 

	if (difference < cmin)  weight = 0.0;
	else if (difference > cmax)  weight = 1.0;
	else weight = (difference - cmin) / (cmax - cmin); 

	return weight; 
}

/**
 * \brief   Copy vector
 *
 * @param[in]   fpI input vector 
 * @param[out]  fpO output vector
 * @param[in]   iLength  length of vector
 */

void fpCopy(float *fpI,float *fpO, int iLength)
{
	memcpy((void *) fpO, (const void *) fpI, iLength * sizeof(float));
}

/**
 * \brief   Linear combination of two vectors:  a * u + b * v 
 *
 * @param[in]   u input vector 
 * @param[in]   v input vector
 * @param[in]   a value 
 * @param[in]   b value
 * @param[out]  w output vector
 * @param[in]   size  length of vector
 */
void fpCombine(float *u,float a,float *v,float b, float *w,  int size)
{
	for(int i = 0; i < size ;i++)   
		w[i]= (a * u[i] + b * v[i]);  
}

/**
 * \brief   Compute gradient magnitude and orientation of an image 
 *
 * @param[in]   tpI				  input image 
 * @param[out]  tpGrad			  output gradient magnitude
 * @param[out]  tpOri			  output gradient orientation
 * @param[in]   iWidth, iHeight   size of the image
 */
void fiComputeImageGradient(float * tpI,float * tpGrad, float * tpOri, int iWidth, int iHeight)
{
	if (!tpI)       { printf("Null input image tpI"); exit(-1);}        

	if (tpGrad)		for (int ii = 0;  ii < iWidth * iHeight; ii++) tpGrad[ii] = 0.0f;
	if (tpOri)      for (int ii = 0;  ii < iWidth * iHeight; ii++) tpOri[ii] = 0.0f;

	for (int ih = 1; ih < iHeight - 1; ih++)
		for (int iw = 1; iw < iWidth - 1; iw++) 
		{ 
			float xgrad = tpI[ih * iWidth + iw + 1] - 
				tpI[ih * iWidth + iw - 1];

			float ygrad = tpI[(ih-1) * iWidth + iw] - 
				tpI[(ih+1) * iWidth + iw];


			if (tpGrad) 
				tpGrad[ih * iWidth + iw] =  
				sqrtf(xgrad * xgrad + ygrad * ygrad);

			if (tpOri) 
				tpOri[ih * iWidth + iw] =  atan2f(-ygrad,xgrad);

		}
}

/**
 * \brief   Build  a  1D Gauss kernel of standard deviation std 
 *                      Length of vector is calculated depending on std
 *
 * @param[in]   std     Gaussian standard deviation 
 * @param[out]  size    length of Gaussian vector
 */
float*  fiFloatGaussKernel(float std, int & size)
{
	int n = 4 * ceilf(std) + 1; 
	size = n;        

	float* u = (float *) malloc(n * sizeof(float));        

	if (n==1)  u[0]=1.0;
	else
	{

		int ishift = (n-1) / 2;

		for (int i=ishift; i < n; i++) 
		{

			float v = (float)(i - ishift) / std;

			u[i] = u[n-1-i] = (float) exp(-0.5*v*v); 

		}
	}    

	// normalize
	float fSum = 0.0f;
	for (int i=0; i < n; i++) fSum += u[i]; 
	for (int i=0; i < n; i++)  u[i] /= fSum;

	return u;
}

/**
 * \brief   1D convolution of a vector with a fixed kernel
 *
 *
 * @param[in]   buffer  input and output vector
 * @param[in]   size    size of input vector
 * @param[in]   kernel  input kernel
 * @param[in]   size    length of kernel
 *
 */
void fiFloatBufferConvolution(float *buffer,float *kernel,int size,int ksize)
{
	for (int i = 0; i < size; i++) 
	{
		float sum = 0.0;
		float *bp = &buffer[i];
		float *kp = &kernel[0];

		int k=0;
		for(;k + 4 < ksize;  bp += 5, kp += 5, k += 5) 
			sum += bp[0] * kp[0] +  bp[1] * kp[1] + bp[2] * kp[2] +
			bp[3] * kp[3] +  bp[4] * kp[4];

		for(; k < ksize; bp++ , kp++, k++)  sum += *bp * (*kp);

		buffer[i] = sum;
	}
}

/**
 * \brief   1D convolution of every row of an image with a fixed kernel
 *
 * @param[in]   u				input image
 * @param[out]  v				output image
 * @param[in]   ksize			length of kernel
 * @param[in]   kernel			input kernel
 * @param[in]   width, height   size of the image
 */
void fiFloatHorizontalConvolution(float *u, float *v, int width, int height, float *kernel, int ksize, int boundary)
{
	int halfsize = ksize / 2;
	int buffersize = width + ksize;
	float *buffer = new float[buffersize];

	for (int r = 0; r < height; r++) 
	{
		// symmetry
		int l = r*width;
		if (boundary == 1)
			for (int i = 0; i < halfsize; i++)
				buffer[i] = u[l + halfsize - 1 - i ];
		else
			for (int i = 0; i < halfsize; i++)
				buffer[i] = 0.0;


		for (int i = 0; i < width; i++)
			buffer[halfsize + i] = u[l + i];


		if (boundary == 1)
			for (int i = 0; i <  halfsize; i++)
				buffer[i + width + halfsize] = u[l + width - 1 - i];
		else 
			for (int i = 0; i <  halfsize; i++)
				buffer[i + width + halfsize] = 0.0;

		fiFloatBufferConvolution(buffer, kernel, width, ksize);
		for (int c = 0; c < width; c++)
			v[r*width+c] = buffer[c];
	}
	
	delete[] buffer;
}

/**
 * \brief   1D convolution of every column of an image with a fixed kernel
 *
 * @param[in]   u				 input image
 * @param[out]  v				 output image
 * @param[in]   ksize			 length of kernel
 * @param[in]   kernel			 input kernel
 * @param[in]   width, height    size of the image
 */

void fiFloatVerticalVonvolution(float *u, float *v, int width, int height, float *kernel,int ksize, int boundary)
{
	int halfsize = ksize / 2;
	int buffersize = height + ksize;
	float *buffer = new float[buffersize];

	for (int c = 0; c < width; c++) {

		if (boundary == 1)
			for (int i = 0; i < halfsize; i++)
				buffer[i] = u[(halfsize-i-1)*width + c];
		else 
			for (int i = 0; i < halfsize; i++)
				buffer[i] = 0.0f;

		for (int i = 0; i < height; i++)
			buffer[halfsize + i] = u[i*width + c];

		if (boundary == 1)
			for (int i = 0; i < halfsize; i++)
				buffer[halfsize + height + i] = 
				u[(height - i - 1)*width+c];
		else
			for (int i = 0; i < halfsize; i++)
				buffer[halfsize + height + i] = 0.0f;

		fiFloatBufferConvolution(buffer, kernel, height, ksize);

		for (int r = 0; r < height; r++)
			v[r*width+c] = buffer[r];
	}

	delete[] buffer;
}

/**
 * \brief   Separable convolution by rows and columns with respective filters
 *          xkernel and ykernel
 *
 * @param[in]   u				 input image
 * @param[out]  v				 output image
 * @param[in]   xksize           length of kernel for row convolution
 * @param[in]   xkernel          input kernel for row convolution
 * @param[in]   yksize			 length of kernel for column convolution
 * @param[in]   ykernel			 input kernel for column convolution
 * @param[in]   width, height    size of the image
 */

void fiSepConvol(float *u,float *v,int width,int height,float *xkernel, int xksize, float *ykernel, int yksize)
{
	int boundary = 1;
	memcpy(v, u, width*height*sizeof(float));
	fiFloatHorizontalConvolution(v, v, width, height, xkernel, xksize, boundary);
	fiFloatVerticalVonvolution(v, v, width, height,  ykernel,  yksize, boundary);
}



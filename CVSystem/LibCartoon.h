
/**
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
 */

/**
 *
 * Buades, A.; Le, T.; Morel, J. M. & Vese, L. 
 * Fast Cartoon + Texture Image Filters Image Processing, 
 * IEEE Transactions on, 2010, 19, 1978-1986
 *
 * @file   LibCartoon.cpp
 * @brief  Cartoon + texture functions
 * @author Antoni Buades <toni.buades@uib.es> 
 *
 * Modified by: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */
#ifndef __Lib_Cartoon__
#define __Lib_Cartoon__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cmath>

/**
 * \brief   Low Pass filter L_sigma f 
 *  The low pass filter is approximated by   L = Id - (Id - G_sigma)^{n *}
 *  That is, the difference between f and G_sigma(f) is convolved again with
 *  G_sigma, and the result of this convolution is put back to f
 *  This approach is iterated niter times
 *
 * @param[in]   input			input image 
 * @param[out]  out				output vector
 * @param[in]   sigma			standard deviation of the low pass filter
 * @param[in]   niter			number of iterations
 * @param[in]   width, height   size of the image
 *
 */
void low_pass_filter(float * input, float * out, float sigma, int niter, int width, int height);

/**
 * \brief  Weighting function w(lambda(x)) being lambda(x) = (r1 - r2) / r1
 *  and w(x) the linear ramp going from 0 to 1 between  lambda = 0.25 and
 *  lambda = 0.5 
 *
 * @param[in]   r1        G_sigma * |Df|        
 * @param[in]   r2        G*|D(L_sigma*f)|
 *
 */
float WeightingFunction(float r1, float r2);

/**
 * \brief   Cartoon + texture decomposition for a single channel image 
 *
 * @param[in]   input			input image 
 * @param[out]  out				output vector
 * @param[in]   sigma			standard deviation of the low pass filter
 * @param[in]   width, height   size of the image
 *
 */

void non_linear_cartoon(float * input, float * out, float sigma, int width, int height);

/**
 * \brief   Cartoon + texture decomposition for a color image 
 *
 * @param[in]   ired, igreen, iblue             input image 
 * @param[out]  ored, ogreen, oblue             output vector
 * @param[in]   sigma                           standard deviation of the 
 *                                              low pass filter
 * @param[in]   width, height                   size of the image
 *
 */
void non_linear_cartoon(float *ired, float *igreen, float *iblue, 
                        float *ored , float *ogreen, float *oblue,  
                        float sigma,  int width, int height);

///
/// Auxiliary functions
///
/**
 * \brief   Copy vector
 *
 * @param[in]   fpI input vector 
 * @param[out]  fpO output vector
 * @param[in]   iLength  length of vector
 */
void fpCopy(float *fpI,float *fpO, int iLength);

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
void fpCombine(float *u,float a,float *v,float b, float *w,  int size);




/**
 * \brief   Compute gradient magnitude and orientation of an image 
 *
 * @param[in]   tpI input image 
 * @param[out]  tpGrad output gradient magnitude
 * @param[out]  tpOri  output gradient orientation
 * @param[in]   iWidth, iHeight   size of the image
 */
void fiComputeImageGradient(float * tpI,float * tpGrad, float * tpOri, int iWidth, int iHeight);

/**
 * \brief  Build a 1D Gauss kernel of standard deviation std 
 *         Length of vector is calculated depending on std
 *
 * @param[in]   std     Gaussian standard deviation 
 * @param[out]  size    length of Gaussian vector
 */
float*  fiFloatGaussKernel(float std, int & size);

/**
 * \brief   Separable convolution by rows and columns with respective filters
 *          xkernel and ykernel
 *
 * @param[in]   u				input image
 * @param[out]  v				output image
 * @param[in]   xksize			length of kernel for row convolution
 * @param[in]   xkernel			input kernel for row convolution
 * @param[in]   yksize			length of kernel for column convolution
 * @param[in]   ykernel			input kernel for column convolution
 * @param[in]   width, height   size of the image
 */
void fiSepConvol(float *u,float *v,int width,int height,float *xkernel, int xksize, float *ykernel, int yksize);

#endif

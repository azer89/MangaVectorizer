
/**
 * Chan-Vese implementation
 * http://www.ipol.im/pub/art/2012/g-cv/
 *
 * Chan, T. & Vese, L. 
 * Active contours without edges Image Processing, 
 * IEEE Transactions on, 2001, 10, 266-277
 *
 * Modified by: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#ifndef CHANVESE_IPOL_H
#define CHANVESE_IPOL_H

#include "SystemParams.h"

// Calculate level set segmentation using ChanVese method
int ChanVese(double* Phi, double* f, int Width, int Height);

// init phi image
void ChanVeseInitPhi(double* Phi, int Width, int Height);

//
void RegionAverages(double* c1, double* c2, double* Phi, double* f, int Width, int Height);

#endif

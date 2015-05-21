
/**
 * Gabor wavelet calculation
 * Manjunath, B. S. & Ma, W. 
 * Texture features for browsing and retrieval of image data 
 * IEEE Transactions on Pattern Analysis and Machine Intelligence (PAMI - Special issue on Digital Libraries), 1996, 18, 837-42
 *
 * Matrix Library
 * http://www.svcl.ucsd.edu/~nikux/test/html/index.html
 *
 * Modified by: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
*/

#ifndef __GABOR_MATRIX__
#define __GABOR_MATRIX__

#include <iostream>
#include <vector>
#include <list>

#define SWAP(a,b) tempr = (a); (a) = (b); (b) = tempr

struct MatrixStruct 
{
	double **data;
	int w;
	int h;
};

class MatrixOperations
{
public:
	// see http://www.svcl.ucsd.edu/~nikux/test/html/index.html for documentation

	static void		nrerror(char *error_text);

	static double*	dvector(int nl, int nh);
	static void		free_vector(float *v, int nl, int nh);

	static float*   vector(int nl, int nh);
	static void		free_dvector(double *v, int nl, int nh);


	static float**	matrix(int nrl,int nrh,int ncl,int nch);
	static void		free_matrix(float **m,int nrl,int nrh,int ncl,int nch);

	static double** dmatrix(int nrl, int nrh, int ncl, int nch);
	static void		free_dmatrix(double **m, int nrl, int nrh, int ncl, int nch);

	static double	log2(double a);

	static void		sort(double *Y, int *I, double *A, int length);
	static void		minimun(double *Y, int *I, double *A, int length);
	static void		Mat_Abs(MatrixStruct *A);
	static void		Mat_Mean(double *mean, MatrixStruct *A);

	static void		Mat_Vector(MatrixStruct *A, float *a);

	static void		Mat_Shift(MatrixStruct *A, MatrixStruct *B, int side);
	static void		Mat_Zeros(MatrixStruct *A);

	static void		CreateMatrix(MatrixStruct **M, int hei, int wid);
	static void		FreeMatrix(MatrixStruct *M);

	static void		Mat_FFT2(MatrixStruct *Output_real, MatrixStruct *Output_imag, MatrixStruct *Input_real, MatrixStruct *Input_imag);
	static void		Mat_IFFT2(MatrixStruct *Output_real, MatrixStruct *Output_imag, MatrixStruct *Input_real, MatrixStruct *Input_imag);
	static void		four2(double **fftr, double **ffti, double **rdata, double **idata, int rs, int cs, int isign);
	static void		four1(double *data, int nn, int isign);

	static void		GetRealPart(MatrixStruct* IMG_real, MatrixStruct* G_real, MatrixStruct* G_imag, MatrixStruct* F_IMG_real, MatrixStruct* F_IMG_imag);
	static void		GetImaginaryPart(MatrixStruct* IMG_imag, MatrixStruct* G_real, MatrixStruct* G_imag, MatrixStruct* F_IMG_real, MatrixStruct* F_IMG_imag);

	static void		Mat_Copy(MatrixStruct *A, MatrixStruct *B, int h_target, int w_target, int h_begin, int w_begin, int h_end, int w_end);
	static void		Mat_MemCopy(MatrixStruct* targetMat, MatrixStruct* sourceMat, int width, int height);
	static void		Mat_Product(MatrixStruct *A, MatrixStruct *B, MatrixStruct *C);
	static void		Mat_Sum(MatrixStruct *A, MatrixStruct *B, MatrixStruct *C);
	static void		Mat_Substract(MatrixStruct *A, MatrixStruct *B, MatrixStruct *C);
};

#endif // !__GABOR_MATRIX__

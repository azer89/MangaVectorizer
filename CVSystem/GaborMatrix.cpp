
#include "stdafx.h"
#include "GaborMatrix.h"

/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

/*
Prints out the error message and exits the system
Parameters:
	error_text 	Error message to print
*/
void MatrixOperations::nrerror(char *error_text)
{
	fprintf(stderr,"Numerical Recipes run-time error...\n");
	fprintf(stderr,"%s\n",error_text);
	fprintf(stderr,"...now exiting to system...\n");
	exit(1);
}

/*
Allocate memory for a double vector that is initialized with zeros. 
The array's starting pointer is adjusted to let the indices run 
from the specified low index to the high index, i.e. 
the indices used to get elements from the array start from nl to nh
Parameters:
	nl 	Low index
	nh 	High index; length of the vector = nh-nl+1
Returns:
	Pointer to the (start of the vector - nl) locations, 
	so that v[nl] points to the first element in the vector
*/
double* MatrixOperations::dvector(int nl, int nh)
{
	double *v;

	v = (double *) calloc((unsigned) (nh-nl+1), sizeof(double));
	if (!v) nrerror("allocation failure in dvector()");
	return v-nl;
}

float* MatrixOperations::vector(int nl, int nh)
{
	float *v;

	v = (float *) calloc((unsigned) (nh-nl+1), sizeof(float));
	if (!v) nrerror("allocation failure in dvector()");
	return v-nl;
}

/*
Free the double vector pointed to by v. It expects a pointer that 
has been adjusted such that v[nl] points to the first element in the vector
Parameters:
	v 	Pointer to the (start of the vector - nl) location
	nl 	Low index
	nh 	High index
*/
void MatrixOperations::free_dvector(double *v, int nl, int nh)
{
	free((char*) (v+nl));
}

void MatrixOperations::free_vector(float *v, int nl, int nh)
{
	free((char*) (v+nl));
}

float** MatrixOperations::matrix(int nrl,int nrh,int ncl,int nch)
{
	int i;
	float **m;

	m=(float **) malloc((unsigned) (nrh-nrl+1)*sizeof(float*));
	if (!m) nrerror("allocation failure 1 in matrix()");
	m -= nrl;

	for(i=nrl;i<=nrh;i++) {
		m[i]=(float *) malloc((unsigned) (nch-ncl+1)*sizeof(float));
		if (!m[i]) nrerror("allocation failure 2 in matrix()");
		m[i] -= ncl;
	}
	return m;
}

void MatrixOperations::free_matrix(float **m,int nrl,int nrh,int ncl,int nch)
{
	int i;

	for(i=nrh;i>=nrl;i--) free((char*) (m[i]+ncl));
	free((char*) (m+nrl));
}

/*
Allocate memory for a double matrix that is initialized with zeros. 
The matrix's starting pointer is adjusted to let the indices run from the specified 
low index to the high index, i.e. the indices used to get elements from 
the array start from nrl to nrh(row index), and ncl to nch(column index)
Parameters:
	nrl 	Low index for rows
	nrh 	High index for rows
	ncl 	Low index got columns
	nch 	High index got columns
Returns:
	Pointer to the (starting element of the matrix - nrl,ncl) 
	locations so that v[nrl][ncl] points to the first element in the matrix
*/
double** MatrixOperations::dmatrix(int nrl, int nrh, int ncl, int nch)
{
	int i;
	double **m;

	m = (double **) calloc((unsigned) (nrh-nrl+1), sizeof(double*));
	if (!m) nrerror("allocation failure 1 in dmatrix()");
	m -= nrl;

	for(i=nrl;i<=nrh;i++) {
		m[i] = (double *) calloc((unsigned) (nch-ncl+1), sizeof(double));
		if (!m[i]) nrerror("allocation failure 2 in dmatrix()");
		m[i] -= ncl;
	}
	return m;
}

/*
Free the double matrix pointed to by v. It expects a pointer 
that has been adjusted such that v[nrl][ncl] points to the first element in the matrix
Parameters:
	v 		Pointer to the (start of the matrix - nrl,ncl) location
	nrl 	Low index for rows
	nrh 	High index for rows
	ncl 	Low index got columns
	nch 	High index got columns
*/
void MatrixOperations::free_dmatrix(double **m, int nrl, int nrh, int ncl, int nch)
{
	int i;

	for(i=nrh;i>=nrl;i--) free((char*) (m[i]+ncl));
	free((char*) (m+nrl));
}


double MatrixOperations::log2(double a)
{
	return log10(a)/log10(2.0);
}

void MatrixOperations::sort(double *Y, int *I, double *A, int length)
{
	int i, j;
	double max, *tmp;

	tmp = (double *) calloc(length, sizeof(double));

	for (i=0;i<length;i++) 
		tmp[i] = A[i];

	max = tmp[0];
	for (i=1;i<length;i++) {
		if (tmp[i] > max) 
			max = tmp[i];
	}

	max = fabs(10*max);

	for (i=0;i<length;i++) {
		Y[i] = tmp[0];
		I[i] = 0;
		for (j=1;j<length;j++) {
			if (tmp[j] < Y[i]) {
				Y[i] = tmp[j];
				I[i] = j;
			}
		}

		tmp[I[i]] = max;
	}

	free(tmp);
}

void MatrixOperations::minimun(double *Y, int *I, double *A, int length)
{
	double min = A[0];
	int index = 0;
	for (int i=1;i<length;i++) 
		if (A[i] < min) {
			min = A[i];
			index = i;
		}

	*Y = min;
	*I = index;
}

void MatrixOperations::Mat_Abs(MatrixStruct *A)
{
	for (int h=0;h<A->h;h++)
		for (int w=0;w<A->w;w++) {
			if (A->data[h][w] < 0)
			    A->data[h][w] = -1.0*(A->data[h][w]);
		}
}

void MatrixOperations::Mat_Mean(double *mean, MatrixStruct *A)
{
	double tmp = 0.0;
	for (int h=0;h<A->h;h++) {
		for (int w=0;w<A->w;w++) {
			tmp += A->data[h][w];
		}
	}

	*mean = tmp/(double) (A->h*A->w);
}

void MatrixOperations::Mat_Vector(MatrixStruct *A, float *a)
{
	for (int h = 0; h < A->h; h++)
		for (int w = 0; w < A->w; w++)
			a[h * A->w + w] = (float) A->data[h][w];
}

void MatrixOperations::Mat_Shift(MatrixStruct *A, MatrixStruct *B, int side)
{
	int h, w;

	for (h=side;h<B->h;h++)
		for (w=side;w<B->w;w++)
			A->data[h-side][w-side] = B->data[h][w];
		
	for (h=side;h<B->h;h++)
		for (w=0;w<side;w++)
			A->data[h-side][B->w-side+w] = B->data[h][w];

	for (h=0;h<side;h++)
		for (w=side;w<B->w;w++)
			A->data[B->h-side+h][w-side] = B->data[h][w];

	for (h=0;h<side;h++)
		for (w=0;w<side;w++)
			A->data[B->h-side+h][B->w-side+w] = B->data[h][w];
}

void MatrixOperations::Mat_Zeros(MatrixStruct *A)
{
	for (int h=0;h < A->h; h++)
		for (int w = 0;w < A->w; w++)
			A->data[h][w] = 0;
}

// Creating matrix with size of img_height * img_width
void MatrixOperations::CreateMatrix(MatrixStruct **M, int img_height, int img_width)
{
	MatrixStruct* tmp = (MatrixStruct *) calloc(1, sizeof(MatrixStruct)); 
	tmp->data = (double **) calloc(img_height, sizeof(double *));

	if (!(tmp->data)) { nrerror("allocation failure in CreateMatrix()"); }

	for (int h = 0; h < img_height; h++) 
	{
		tmp->data[h] = (double *) calloc(img_width, sizeof(double));
		if (!(tmp->data[h])) { nrerror("allocation failure in CreateMatrix()"); }
	}

	tmp->h = img_height;
	tmp->w = img_width;
	
	*M = tmp;
}

// Deallocating matrix
void MatrixOperations::FreeMatrix(MatrixStruct *M)
{
	int hei = M->h;
	for (int h = 0; h<hei; h++) { free(M->data[h]); }
	free(M->data);
	free(M);
}

/*
http://www.svcl.ucsd.edu/~nikux/test/html/documented_2fft_8c-source.html

Compute the 2-D FFT of data
Parameters:
	Output_real 	Real part of the result
	Output_imag 	Imaginary part of the result
	Input_real 		The matrix containing the real part of input
	Output_real 	The matrix containing the imaginary part of input
*/
void MatrixOperations::Mat_FFT2(MatrixStruct *Output_real, 
								MatrixStruct *Output_imag, 
								MatrixStruct *Input_real, 
								MatrixStruct *Input_imag)
{
	int xs = Input_real->h;
	int ys = Input_real->w;

	double** R  = dmatrix(1,xs,1,ys);
	double** I  = dmatrix(1,xs,1,ys);
	double** Fr = dmatrix(1,xs,1,ys);
	double** Fi = dmatrix(1,xs,1,ys);

	for (int i = 1;i <= Input_real->h; i++)
	{
		for (int j = 1;j <= Input_real->w; j++) 
		{
			R[i][j] = Input_real->data[i-1][j-1];
			I[i][j] = Input_imag->data[i-1][j-1];
		}
	}

	four2(Fr, Fi, R, I, xs, ys, 1);         /* 2-D FFT */

	for (int i = 1; i <= Input_real->h; i++) 
	{
		for (int j=1;j<=Input_real->w;j++) 
		{
			Output_real->data[i-1][j-1] = Fr[i][j];
			Output_imag->data[i-1][j-1] = Fi[i][j];
		}
	}

	free_dmatrix(R,  1, xs, 1, ys);
	free_dmatrix(I,  1, xs, 1, ys);   
	free_dmatrix(Fr, 1, xs, 1, ys);
	free_dmatrix(Fi, 1, xs, 1, ys);   
}

/*
http://www.svcl.ucsd.edu/~nikux/test/html/documented_2fft_8c-source.html

Compute the 2-D IFFT of data
Parameters:
	Output_real 	Real part of the result
	Output_imag 	Imaginary part of the result
	Input_real 		The matrix containing the real part of input
	Output_real 	The matrix containing the imaginary part of input
*/
void MatrixOperations::Mat_IFFT2(MatrixStruct *Output_real, MatrixStruct *Output_imag, MatrixStruct *Input_real, MatrixStruct *Input_imag)
{
	int xs = Input_real->h;
	int ys = Input_real->w;

	double** R  = dmatrix(1,xs,1,ys);
	double** I  = dmatrix(1,xs,1,ys);
	double** Fr = dmatrix(1,xs,1,ys);
	double** Fi = dmatrix(1,xs,1,ys);

	for (int i=1;i<=Input_real->h;i++)
	{
		for (int j=1;j<=Input_real->w;j++) 
		{
			R[i][j] = Input_real->data[i-1][j-1];
			I[i][j] = Input_imag->data[i-1][j-1];
		}
	}

    four2(Fr, Fi, R, I, xs, ys, -1);         /* 2-D IFFT */

	double NN = (double) (xs*ys);
	for (int i=1;i<=Input_real->h;i++)
	{
		for (int j=1;j<=Input_real->w;j++) 
		{
			Output_real->data[i-1][j-1] = Fr[i][j]/NN;
			Output_imag->data[i-1][j-1] = Fi[i][j]/NN;
		}
	}

	free_dmatrix(R,  1, xs, 1, ys);
	free_dmatrix(I,  1, xs, 1, ys);   
	free_dmatrix(Fr, 1, xs, 1, ys);
	free_dmatrix(Fi, 1, xs, 1, ys);   
}

/*
http://www.svcl.ucsd.edu/~nikux/test/html/documented_2fft_8c-source.html

Compute the 2-D( forward or inverse) fourier transform
Compute the 2-D fourier transform of data with real part stored in "rdata" 
and imaginary part in "idata" with size "rs" x "cs". The result is in "fftr" and "ffti". 
The isign is "isign" = 1 forward, and "isign" = -1 inverse
Parameters:
	fftr 	Real part of the result
	ffri 	Imaginary part of the result
	rdata 	The matrix containing the real part of input
	idata 	The matrix containing the imaginary part of input
	rs 		# of rows in rdata,idata
	cs 		# of cols in rdata,idata
	isign 	The isign represents: "isign" = 1 forward, and "isign" = -1 inverse
*/
void MatrixOperations::four2(double **fftr, double **ffti, double **rdata, double **idata, int rs, int cs, int isign)
{
        double **T, *tmp1, *tmp2;
        int i, j;

        tmp1 = dvector(1,2*cs);
        tmp2 = dvector(1,2*rs);
        T = dmatrix(1,2*rs,1,cs);

        for (i=1;i<=rs;i++) 
		{
            for (j=1;j<=cs;j++) 
			{
                tmp1[j*2-1] = rdata[i][j];
                tmp1[j*2] = idata[i][j];
            }
            four1(tmp1, cs, isign);
            for (j=1;j<=cs;j++) 
			{
                T[i*2-1][j] = tmp1[j*2-1];
                T[i*2][j] = tmp1[j*2];
            }
        }

        for (i=1;i<=cs;i++) 
		{
            for (j=1;j<=rs;j++) 
			{
                tmp2[j*2-1] = T[j*2-1][i];
                tmp2[j*2] = T[j*2][i];
            }
            four1(tmp2,rs,isign);
            for (j=1;j<=rs;j++) 
			{
                fftr[j][i] = tmp2[j*2-1];
                ffti[j][i] = tmp2[j*2];
            }
        }
        free_dvector(tmp1, 1, 2*cs);
        free_dvector(tmp2, 1, 2*rs);
        free_dmatrix(T, 1, 2*rs, 1, cs); 
}

/*
http://www.svcl.ucsd.edu/~nikux/test/html/documented_2fft_8c-source.html

Compute the 1-D FFT/IFFT.
Compute the 1-D FFT or IFFT of the given data, using the butterfly architecture.
Parameters:
	data 	The input vector which needs to be transformed
	nn 	The base for the FFT/IFFT
	isign 	The isign represents: "isign" = 1 forward, and "isign" = -1 inverse
*/
void MatrixOperations::four1(double *data, int nn, int isign)
{
	int n, mmax, m, j, istep, i;
	double wtemp, wr, wpr, wpi, wi, theta;
	double tempr, tempi;

	n = nn << 1;
	j = 1;
	for (i=1;i<n;i+=2) 
	{
		if (j > i) 
		{
			SWAP(data[j],data[i]);
			SWAP(data[j+1],data[i+1]);
		}
		m = n >> 1;
		while (m >= 2 && j > m) 
		{
			j -= m;
			m >>= 1;
		}
		j += m;
	}
	mmax = 2;
	while (n > mmax) 
	{
		istep = 2*mmax;
		theta = 6.28318530717959 / (isign*mmax);
		wtemp = sin(0.5*theta);
		wpr = -2.0*wtemp*wtemp;
		wpi = sin(theta);
		wr = 1.0;
		wi = 0.0;
		for (m=1;m<mmax;m+=2) 
		{
			for (i=m;i<=n;i+=istep) 
			{
				j = i+mmax;
				tempr = wr*data[j]-wi*data[j+1];
				tempi = wr*data[j+1]+wi*data[j];
				data[j] = data[i]-tempr;
				data[j+1] = data[i+1]-tempi;
				data[i] += tempr;
				data[i+1] += tempi;
			}
			wr = (wtemp=wr)*wpr-wi*wpi+wr;
			wi = wi*wpr+wtemp*wpi+wi;
		}
		mmax = istep;
	}
}

void MatrixOperations::Mat_MemCopy(MatrixStruct* targetMat, MatrixStruct* sourceMat, int width, int height)
{
	for(int a = 0; a < height; a++)
		memcpy(targetMat->data[a], sourceMat->data[a], width * sizeof(double));
}

void MatrixOperations::Mat_Copy(MatrixStruct *A,	// target
								MatrixStruct *B,	// source
								int h_target, // h start index of target
								int w_target, // w start index of target
								int h_begin,	// h start index of source
								int w_begin,	// w start index of source
								int h_end,	// h end index of source
								int w_end)	// w end index of source
{
	int i, j, h, w, h_done, w_done;

	if ((h_target >= 0) && (h_target < A->h) && (w_target >= 0) && (w_target < A->w)) 
	{
		if ((h_begin >= 0) && (h_begin < B->h) && (w_begin >= 0) && (w_begin < B->w)) 
		{
			h = h_end-h_begin+1;
			w = w_end-w_begin+1;

			if ((h >= 1) && (w >= 1)) 
			{
				h_done = h_target+h-1;
				w_done = w_target+w-1;

				if ((h_done < A->h) && (w_done < A->w)) 
				{
					for (i=0;i<h;i++) 
					{
						for (j=0;j<w;j++) 
						{
							A->data[i+h_target][j+w_target] = B->data[i+h_begin][j+w_begin];
						}
					}
				}
			}
		}
	}
	else 
	{
		printf("matrix dimension error!\n");
		exit(1);
	}
}

void MatrixOperations::GetRealPart(MatrixStruct* IMG_real, MatrixStruct* G_real, MatrixStruct* G_imag, MatrixStruct* F_IMG_real, MatrixStruct* F_IMG_imag)
{
	for (int h = 0; h < IMG_real->h; h++) 
		for (int w = 0; w < IMG_real->w; w++) 
			IMG_real->data[h][w] = (G_real->data[h][w] * F_IMG_real->data[h][w]) - 
								   (G_imag->data[h][w] * F_IMG_imag->data[h][w]);
}

void MatrixOperations::GetImaginaryPart(MatrixStruct* IMG_imag, MatrixStruct* G_real, MatrixStruct* G_imag, MatrixStruct* F_IMG_real, MatrixStruct* F_IMG_imag)
{
	for (int h = 0; h < IMG_imag->h; h++) 
		for (int w = 0; w < IMG_imag->w; w++) 
			IMG_imag->data[h][w] = (G_real->data[h][w] *  F_IMG_imag->data[h][w]) + 
								   (G_imag->data[h][w] *  F_IMG_real->data[h][w]);
}

void MatrixOperations::Mat_Product(MatrixStruct *A, MatrixStruct *B, MatrixStruct *C)
{
	for (int h=0;h<A->h;h++) 
		for (int w=0;w<A->w;w++) 
			A->data[h][w] = B->data[h][w]*C->data[h][w];
}

void MatrixOperations::Mat_Sum(MatrixStruct *A, MatrixStruct *B, MatrixStruct *C)
{
	for (int h=0;h<A->h;h++) 
		for (int w=0;w<A->w;w++) 
			A->data[h][w] = B->data[h][w] + C->data[h][w];
}

void MatrixOperations::Mat_Substract(MatrixStruct *A, MatrixStruct *B, MatrixStruct *C)
{
	for (int h=0;h<A->h;h++) 
		for (int w=0;w<A->w;w++) 
			A->data[h][w] = B->data[h][w] - C->data[h][w];
}

/*void MatrixOperations::Mat_Fliplr(GaborMatrix *A)
{
	GaborMatrix *tmp;
	int h, w;

	CreateMatrix(&tmp, A->height, A->width);
	
	for (h=0;h<A->height;h++) {
		for (w=0;w<A->width;w++) {
			tmp->data[h][w] = A->data[h][(A->width)-w-1];
		}
	}

	for (h=0;h<A->height;h++) {
		for (w=0;w<A->width;w++) {
			A->data[h][w] = tmp->data[h][w];
		}
	}

	FreeMatrix(tmp);
}*/

/*void MatrixOperations::Mat_Flipud(GaborMatrix *A)
{
	GaborMatrix *tmp;
	int h, w;

	CreateMatrix(&tmp, A->height, A->width);
	
	for (h=0;h<A->height;h++) {
		for (w=0;w<A->width;w++) {
			tmp->data[h][w] = A->data[(A->height)-h-1][w];
		}
	}

	for (h=0;h<A->height;h++) {
		for (w=0;w<A->width;w++) {
			A->data[h][w] = tmp->data[h][w];
		}
	}

	FreeMatrix(tmp);
}*/
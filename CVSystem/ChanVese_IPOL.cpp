
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "ChanVese_IPOL.h"

int ChanVese(double* Phi, double* f, int Width, int Height)
{
	std::cout << "Level set commenced...\n";

	const long NumPixels = ((long)Width) * ((long)Height);
	//const long NumEl = NumPixels * NumChannels;
	const long NumEl = NumPixels;
	const double *fPtr; 
	//	         *fPtr2;

	double PhiDiffNorm, 
		   PhiDiff;

	double *PhiPtr, 
		   *c1 = 0, 
		   *c2 = 0;

	double c1Scalar, 
		   c2Scalar, 
		   Mu, 
		   Nu, 
		   Lambda1, 
		   Lambda2, 
		   dt;

	double PhiLast, 
		   Delta, 
		   PhiX, 
		   PhiY, 
		   IDivU, 
		   IDivD, 
		   IDivL, 
		   IDivR;
	
	//double Temp1, 
	//	   Temp2, 
	double  Dist1, 
		   Dist2, 
		   PhiTol;
	
	int Iter, 
		i, 
		j, 
	//	Channel, 
		MaxIter, 
		Success = 2;
	
	int iu, 
		id, 
		il, 
		ir;

	if(!Phi || !f || Width <= 0 || Height <= 0)
		return 0;

	//if(!Opt)
	//	Opt = &DefaultChanVeseOpt;

	Mu			= SystemParams::cv_ip_Mu;
	Nu			= SystemParams::cv_ip_Nu;
	Lambda1		= SystemParams::cv_ip_Lambda1;
	Lambda2		= SystemParams::cv_ip_Lambda2;
	dt			= SystemParams::cv_ip_dt;
	MaxIter		= SystemParams::cv_ip_MaxIter;
	PhiTol		= SystemParams::cv_ip_Tol;
	PhiDiffNorm = (PhiTol > 0) ? PhiTol * 1000 : 1000;

	//if(NumChannels > 1)
	//{
	//	if(!(c1 = Malloc(sizeof(num)*NumChannels)) 
	//		|| !(c2 = Malloc(sizeof(num)*NumChannels)))
	//		return 0;
	//}
	//else
	//{
	c1 = &c1Scalar;
	c2 = &c2Scalar;
	//}

	RegionAverages(c1, c2, Phi, f, Width, Height);

	//if(PlotFun)
	//	if(!PlotFun(0, 0, PhiDiffNorm, c1, c2, Phi,
	//		Width, Height, NumChannels, Opt->PlotParam))
	//		goto Done;

	for(Iter = 1; Iter <= MaxIter; Iter++)
	{
		PhiPtr = Phi;
		fPtr = f;
		PhiDiffNorm = 0;

		for(j = 0; j < Height; j++)
		{
			iu = (j == 0) ? 0 : -Width;
			id = (j == Height - 1) ? 0 : Width;

			for(i = 0; i < Width; i++, PhiPtr++, fPtr++)
			{
				il = (i == 0) ? 0 : -1;
				ir = (i == Width - 1) ? 0 : 1;

				Delta = dt/(M_PI*(1.0 + PhiPtr[0]*PhiPtr[0]));
				PhiX  = PhiPtr[ir] - PhiPtr[0];
				PhiY  = (PhiPtr[id] - PhiPtr[iu])/2.0;
				IDivR = (double)(1.0 / sqrt(DIVIDE_EPS + PhiX*PhiX + PhiY*PhiY));
				PhiX  = PhiPtr[0] - PhiPtr[il];
				IDivL = (double)(1.0 / sqrt(DIVIDE_EPS + PhiX*PhiX + PhiY*PhiY));
				PhiX  = (PhiPtr[ir] - PhiPtr[il])/2.0;
				PhiY  =  PhiPtr[id] - PhiPtr[0];
				IDivD = (double)(1.0 / sqrt(DIVIDE_EPS + PhiX*PhiX + PhiY*PhiY));
				PhiY  = PhiPtr[0] - PhiPtr[iu];
				IDivU = (double)(1.0 / sqrt(DIVIDE_EPS + PhiX*PhiX + PhiY*PhiY));

				//if(NumChannels == 1)
				//{
				Dist1 = fPtr[0] - c1Scalar;
				Dist2 = fPtr[0] - c2Scalar;

				Dist1 *= Dist1;
				Dist2 *= Dist2;
				//}
				/*else    
				{
					Dist1 = Dist2 = 0.0;

					for(Channel = 0, fPtr2 = fPtr; 
						Channel < NumChannels; Channel++, fPtr2 += NumPixels)
					{
						Temp1 = fPtr2[0] - c1[Channel];
						Temp2 = fPtr2[0] - c2[Channel];
						Dist1 += Temp1*Temp1;
						Dist2 += Temp2*Temp2;
					}
				}*/

				/* Semi-implicit update of phi at the current point */
				PhiLast = PhiPtr[0];

				PhiPtr[0] = (PhiPtr[0] + Delta * (
					Mu * (PhiPtr[ir] * IDivR + PhiPtr[il] * IDivL
					+ PhiPtr[id] * IDivD + PhiPtr[iu] * IDivU)
					- Nu - Lambda1 * Dist1 + Lambda2 * Dist2) ) /
					(1 + Delta * Mu * (IDivR + IDivL + IDivD + IDivU));
				
				PhiDiff = (PhiPtr[0] - PhiLast);
				
				PhiDiffNorm += PhiDiff * PhiDiff;
			}
		}

		PhiDiffNorm = sqrt(PhiDiffNorm/NumEl);        
		RegionAverages(c1, c2, Phi, f, Width, Height);

		if(Iter >= 2 && PhiDiffNorm <= PhiTol) break;

		/*if(PlotFun)
			if(!PlotFun(0, Iter, PhiDiffNorm, c1, c2, Phi,
				Width, Height, NumChannels, Opt->PlotParam))
				goto Done;*/
	}

	std::cout << "Level set done in " << Iter << " iterations\n";

	Success = (Iter <= MaxIter) ? 1:2;

	//if(PlotFun)
	//	PlotFun(Success, (Iter <= MaxIter) ? Iter:MaxIter, 
	//	PhiDiffNorm, c1, c2, Phi, 
	//	Width, Height, NumChannels, Opt->PlotParam);

/*Done:        
	if(NumChannels > 1)
	{
		Free(c2);
		Free(c1);
	}*/

	return Success;    
}

void ChanVeseInitPhi(double* Phi, int Width, int Height)
{
	for(int j = 0; j < Height; j++)
		for(int i = 0; i < Width; i++)
			*(Phi++) = (double)(sin(i*M_PI/5.0)*sin(j*M_PI/5.0));
}

void RegionAverages(double* c1, double* c2, double* Phi, double* f, int Width, int Height)
{
	const long NumPixels = ((long)Width) * ((long)Height);
	double Sum1 = 0, Sum2 = 0;
	long n;
	long Count1 = 0, Count2 = 0;
	//int Channel;

	for(n = 0; n < NumPixels; n++)
	{
		if(Phi[n] >= 0)
		{
			Count1++;
			Sum1 += f[n];
		}
		else
		{
			Count2++;
			Sum2 += f[n];
		}
	}

	c1[0] = (Count1) ? (Sum1/Count1) : 0;
	c2[0] = (Count2) ? (Sum2/Count2) : 0;
}



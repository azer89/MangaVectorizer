
/**
 * Type of the curve,
 * read Blinn-Loop paper for more information
 *
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __CURVETYPE__
#define __CURVETYPE__

typedef enum CurveType
{
	CURVE_TYPE_UNKNOWN		= 0,
	CURVE_TYPE_SERPENTINE	= 1,	
	CURVE_TYPE_LOOP			= 2,	
	CURVE_TYPE_CUSP			= 3,
	CURVE_TYPE_QUADRATIC	= 4,
	CURVE_TYPE_LINE			= 5,
};

#endif
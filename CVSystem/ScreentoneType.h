
/**
 * Type of screentone
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __Screentone_Type__
#define __Screentone_Type__


namespace CVSystem
{
	typedef enum
	{
		SCREENTONE_DOTS		= 0,	// Param extraction supported,	   rendering supported
		SCREENTONE_LINES	= 1,	// Param extraction supported,	   rendering supported
		SCREENTONE_GRID		= 2,	// Param extraction supported,	   rendering supported

		SCREENTONE_RANDOM	= 3,	// Param extraction not supported, rendering supported
		SCREENTONE_NOISE	= 4,	// Param extraction not supported, rendering supported

		NUM_SCREENTONE = 5,
	}
	ScreentoneType;
}
#endif
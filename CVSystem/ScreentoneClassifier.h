
/**
 * Screentone type classification
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __SCREENTONEGENERATOR_H__
#define __SCREENTONEGENERATOR_H__

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "ScreentoneData.h"
#include "ScreentoneType.h"

namespace CVSystem
{
	class ScreentoneClassifier
	{
	public:
		// constructor
		ScreentoneClassifier() : 
			tonesLoaded(false),
			max_neighbor(5),
			scale_factor(1.0)
		{
		}

		// destructor
		~ScreentoneClassifier(){}

		// get calculated parameters
		std::vector<double> GetParameters(std::string fileName, int* mask);

		// load texture database
		void LoadTones();

		// read precomputed params from file
		bool ReadParams(std::vector<double>& params, std::string filename);

		// write precomputed params from file
		void WriteParams(std::vector<double> params, std::string filename);

	private:
		// database
		std::vector<ScreentoneData> scDataList;

		// is database loaded
		bool tonesLoaded;

		int max_neighbor;
		double scale_factor;
	};
}

#endif

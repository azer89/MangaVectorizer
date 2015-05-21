
/**
 * Representing data of a screentone:
 *	- parameters
 *	- lbp histogram
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __SCREENTONEDATA_H__
#define __SCREENTONEDATA_H__

#include <string>
#include "UtilityFunctions.h"


namespace CVSystem
{
	// this class is representation of a single data in texture database
	struct ScreentoneData
	{
	public:
		// Constructor
		ScreentoneData(std::string filename, cv::Mat img, cv::Mat lbpImg, cv::Mat hist)
		{
			this->filename = filename;
			this->img = img;
			this->lbpImg = lbpImg;
			this->hist = hist;

			//std::vector<int> params;
			std::string wExt = this->filename.substr(0, this->filename.size() - 4);
			std::vector<std::string> paramsArr = UtilityFunctions::split(wExt, '_');

			for(size_t a = 0; a < paramsArr.size(); a++)
				this->params.push_back(atoi(paramsArr[a].c_str()));
		}

		// screentone type
		int GetType() { return this->params[0]; }

		// screntone parameters
		std::vector<int> GetParameters() { return this->params; }
		
		std::vector<int> params;
		std::string filename;
		cv::Mat img;
		cv::Mat lbpImg;
		cv::Mat hist;
	};
}

#endif

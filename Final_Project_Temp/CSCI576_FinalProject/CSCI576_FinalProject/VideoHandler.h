#pragma once

#include "DominantColorList.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <list>
#include <string>
#include <cmath>

using namespace cv;
using std::string;

class VideoHandler{

	private:
		int Width = 352;
		int Height = 288;

		string filename;
		DominantColorList data;

	public:
		VideoHandler();
		void setFileName(string in) { filename = in; };
		void processFrames();
		DominantColorList getProcessedData() { return data; };
};


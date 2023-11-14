#pragma once

#include "SceneData.h"
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
		int maxDeviation = 300000;

		string filename;
		std::list<SceneData> scenes;
	public:
		VideoHandler();
		void setFileName(string in) { filename = in; };
		void processFrames();

		std::list<SceneData> getScenes() { return scenes; };
		void collapseShortClips();
};


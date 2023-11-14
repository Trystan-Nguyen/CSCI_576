#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

class SceneData{
private:
	int Width = 352;
	int Height = 288;

	int* hueHist;
	int frameCount;
	int startFrameIndex;

	// TEMP
	Mat firstFrameReference_TEMP;
public:
	SceneData();
	~SceneData();
	void addFrame(int* image);
	int getFrameCount() { return frameCount; };
	void setStartFrameIndex(int i) { startFrameIndex = i; };
	int getStartFrameIndex() { return startFrameIndex; };
	int* getHueHist() { return hueHist; };
	void mergeSceneData(SceneData i);

	// TEMP
	void setFirstFrame(Mat t) { firstFrameReference_TEMP = t; };
	Mat getFirstFrame() { return firstFrameReference_TEMP; };
};


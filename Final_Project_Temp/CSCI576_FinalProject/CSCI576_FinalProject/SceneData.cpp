#include "SceneData.h"

using namespace std;

SceneData::SceneData() {
	hueHist = new int[360];
	for (int i = 0; i < 360; ++i) hueHist[0] = 0;
	frameCount = 0;
	startFrameIndex = -1;
}

SceneData::~SceneData() {
	if (!hueHist) delete[] hueHist;
}

void SceneData::addFrame(int* img) {
	for (int i = 0; i < Width * Height/4; ++i) {
		++hueHist[img[i]];
	}
	++frameCount;
}

void SceneData::mergeSceneData(SceneData toMerge) {
	int* toMergeHueHist = toMerge.getHueHist();
	for (int i = 0; i < 360; ++i) {
		hueHist[i] += toMergeHueHist[i];
	}
	frameCount += toMerge.getFrameCount();
}


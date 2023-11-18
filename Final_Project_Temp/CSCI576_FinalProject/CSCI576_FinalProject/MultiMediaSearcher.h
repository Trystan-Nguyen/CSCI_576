#pragma once
#include "VideoHandler.h"
#include "AudioHandler.h"

class MultiMediaSearcher
{
private:
	string queryVideo;
	string queryAudio;

	string result;
	int index;
	int totalFrames;
	Mat firstFrame;

public:
	MultiMediaSearcher();
	void setVideoFilePath(string s) { queryVideo = s; };
	void setAudioFilePath(string s) { queryAudio = s; };
	void search();

	int getIndex() { return index; };
	int getTotalFrames() { return totalFrames; };
	string getSrcVideo() { return result; };

	Mat getTargetFrame() { return firstFrame; };

};
#pragma once
#include "VideoHandler.h"

class MultiMediaSearcher
{
private:
	String queryVideo;
	String queryAudio;

	String result;
	int index;

public:
	MultiMediaSearcher();
	void setVideoFilePath(string s) { queryVideo = s; };
	void setAudioFilePath(string s) { queryAudio = s; };
	void search();

};


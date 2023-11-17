#pragma once
#include "AudioFile.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class AudioHandler{
private:
	string filepath;
	vector<double> audioByFrame;
public:
	AudioHandler();
	void setFilePath(string s) { filepath = s; };
	void processAudio();
	vector<double>* getAudioData() { return &audioByFrame; };

	int compareAudio(AudioHandler* subsignal);

	void dumpData(string filepath);
	void populateData(string filepath);
};


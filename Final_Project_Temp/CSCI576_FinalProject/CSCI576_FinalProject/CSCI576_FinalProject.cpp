// CSCI576_FinalProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <string>
#include "MultiMediaSearcher.h"
#include "AudioHandler.h"


int main(int argc, char* argv[])
{
	/*
	// Build Preprocessed Dataset (Change img RowCol iter)
	string dataLocation = "PreprocessData/video";
	string vFilePath = "Data/Videos/video";
	string aFilePath = "Data/Audios/video";
	for (int i = 1; i <= 20; ++i) {
		printf("Start %d\n", i);
		VideoHandler srcVideo = VideoHandler();
		srcVideo.setFileName(vFilePath+to_string(i)+".mp4");
		srcVideo.processFrames();
		srcVideo.getProcessedData().dumpData(dataLocation + to_string(i));

		AudioHandler srcAudio = AudioHandler();
		srcAudio.setFilePath(aFilePath + to_string(i) + ".wav");
		srcAudio.processAudio();
		srcAudio.dumpData(dataLocation + to_string(i));
		printf("End %d\n\n", i);
	}
	return 0;
	//*/
	
	/** Quick Test
	string query = "11_1";
	MultiMediaSearcher dataGetter = MultiMediaSearcher();
	dataGetter.setVideoFilePath("Data/Queries/video" + query + ".mp4");
	dataGetter.setAudioFilePath("Data/Queries/audios/video" + query + ".wav");
	dataGetter.search();
	//*/
	
	//* Commandline Args
	string queryVideo = argv[1];
	string queryAudio = argv[2];

	MultiMediaSearcher dataGetter = MultiMediaSearcher();
	dataGetter.setVideoFilePath(queryVideo);
	dataGetter.setAudioFilePath(queryAudio);
	dataGetter.search();
	//*/

	int i = dataGetter.getIndex();
	int tf = dataGetter.getTotalFrames();
	string src = dataGetter.getSrcVideo();
	
	if (i != -1) {
		string cmd = "python MediaPlayer.py " + src + " " + to_string(i) + " " + to_string(tf);
		system(cmd.c_str());
	}
	
}
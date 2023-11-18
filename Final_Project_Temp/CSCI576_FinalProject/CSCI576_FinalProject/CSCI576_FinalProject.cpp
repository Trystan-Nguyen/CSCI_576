// CSCI576_FinalProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <string>
#include "MultiMediaSearcher.h"
#include "AudioHandler.h"


int main(int argc, char* argv[])
{
	/** Quick Test
	string query = "11_1";
	MultiMediaSearcher dataGetter = MultiMediaSearcher();
	dataGetter.setVideoFilePath("Data/Queries/video" + query + ".mp4");
	dataGetter.setAudioFilePath("Data/Queries/audios/video" + query + ".wav");
	dataGetter.search();
	*/

	string queryVideo = argv[1];
	string queryAudio = argv[2];

	MultiMediaSearcher dataGetter = MultiMediaSearcher();
	dataGetter.setVideoFilePath(queryVideo);
	dataGetter.setAudioFilePath(queryAudio);
	dataGetter.search();
	
	int i = dataGetter.getIndex();
	int tf = dataGetter.getTotalFrames();
	string src = dataGetter.getSrcVideo();
	
	/*
	if (i != -1) {
		namedWindow("FrameStartRef", 1);
		Mat frame = dataGetter.getTargetFrame();
		imshow("FrameStartRef", frame);
		waitKey();
	}
	*/
	if (i != -1) {
		string cmd = "python MediaPlayer.py " + src + " " + to_string(i) + " " + to_string(tf);
		system(cmd.c_str());
	}
	
}
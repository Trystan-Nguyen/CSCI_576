// CSCI576_FinalProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "MultiMediaSearcher.h"

#include "AudioHandler.h"

int main()
{
	//MultiMediaSearcher dataGetter = MultiMediaSearcher();
	//dataGetter.search();

	AudioHandler a = AudioHandler();
	//a.setFilePath("Data/Audios/video1.wav");
	a.setFilePath("Data/Queries/audios/video1_1.wav");
	a.processAudio();
	//a.dumpData("TestFolder/video1");

	AudioHandler src = AudioHandler();
	src.populateData("TestFolder/video1");


	int results = src.compareAudio(&a);
	printf("Results: %d\n", results);
}
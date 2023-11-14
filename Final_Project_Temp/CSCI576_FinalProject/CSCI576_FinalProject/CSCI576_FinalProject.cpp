// CSCI576_FinalProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "VideoHandler.h";
#include "SceneData.h"

int main()
{
	VideoHandler videoObj = VideoHandler::VideoHandler();
	//videoObj.setFileName("D:/GitRepos/CSCI_576/Final_Project_Temp/Data/Queries/video2_1.mp4");
	videoObj.setFileName("D:/GitRepos/CSCI_576/Final_Project_Temp/Data/Videos/video5.mp4");
	videoObj.processFrames();
	//videoObj.collapseShortClips();
	
	for (SceneData i : videoObj.getScenes()) {
		printf("First Index: %d\n", i.getStartFrameIndex());
		imshow("w", i.getFirstFrame());
		waitKey();
	}
}
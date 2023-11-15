// CSCI576_FinalProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "VideoHandler.h"

#include <iostream>
#include <fstream>

void writeToFile(string filePath, DominantColorList* obj) {
	ofstream file_obj;
	file_obj.open(filePath, ios::app);
	file_obj.write((char*)&obj, sizeof(obj));
	file_obj.close();
}

DominantColorList readFromFile(string filePath) {
	ifstream file_obj;
	file_obj.open(filePath, ios::in);
	DominantColorList retObj;

	file_obj.read((char*)&retObj, sizeof(retObj));
	file_obj.close();

	return retObj;
}

int main()
{
	/*
	printf("START-------------------------------------\n");
	printf("Start Load Src\n");
	VideoHandler videoObjSrc = VideoHandler::VideoHandler();
	videoObjSrc.setFileName("D:/GitRepos/CSCI_576/Final_Project_Temp/Data/Videos/video4.mp4");
	videoObjSrc.processFrames();
	DominantColorList signal = videoObjSrc.getProcessedData();
	printf("End Load Src\n");
	*/
	/*
	printf("Start Load Query\n");
	VideoHandler videoObj = VideoHandler::VideoHandler();
	videoObj.setFileName("D:/GitRepos/CSCI_576/Final_Project_Temp/Data/Queries/video4_1.mp4");
	videoObj.processFrames();
	DominantColorList subsignal = videoObj.getProcessedData();
	printf("End Load Query\n");
	//*/

	//subsignal.dumpData("PreprocessData/test");
	//**
	DominantColorList testRead = DominantColorList();
	testRead.populateData("PreprocessData/test");
	
	if(testRead.getHueInformation()[17]==1)
		printf("Size: %d\n", testRead.getHueOccurenceSize());
	else 
		printf("FALSE\n", testRead.getHueOccurenceSize());
	//*/
	/*
	printf("Start Compare\n");
	int test = subsignal.containsSubset(&testRead);
	printf("RESULT: %d\n", test);
	printf("End Compare\n");
	//*/
}
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
	printf("START-------------------------------------\n");
	/*
	printf("Start Load Src\n");
	VideoHandler videoObjSrc = VideoHandler::VideoHandler();
	videoObjSrc.setFileName("D:/GitRepos/CSCI_576/Final_Project_Temp/Data/Videos/video11.mp4");
	videoObjSrc.processFrames();
	DominantColorList signal = videoObjSrc.getProcessedData();
	printf("End Load Src\n");
	signal.dumpData("PreprocessData/video11");

	//*/
	//*
	printf("Start Load Query\n");
	VideoHandler videoObj = VideoHandler::VideoHandler();
	videoObj.setFileName("D:/GitRepos/CSCI_576/Final_Project_Temp/Data/Queries/video11_1.mp4");
	videoObj.processFrames();
	DominantColorList subsignal = videoObj.getProcessedData();
	printf("End Load Query\n");

	//subsignal.dumpData("PreprocessData/test_query");
	//*/

	printf("\n\n----------------------------------------------------------\n\n");
	string filePath = "PreprocessData/video";
	for (int i = 1; i < 12; ++i) {
	//int i = 11;
		DominantColorList signal = DominantColorList();
		string f = filePath + std::to_string(i);
		signal.populateData(f);

		int test = signal.containsSubset(&subsignal);
		printf("RESULT of %d: %d\n", i, test);
	}

	/**
	DominantColorList testRead = DominantColorList();
	testRead.populateData("PreprocessData/video4");
	printf("Size: %d\n", testRead.getHueOccurenceSize());
	//*/

	/*
	printf("Start Compare\n");
	int test = testRead.containsSubset(&subsignal);
	printf("RESULT: %d\n", test);
	printf("End Compare\n");
	//*/
}
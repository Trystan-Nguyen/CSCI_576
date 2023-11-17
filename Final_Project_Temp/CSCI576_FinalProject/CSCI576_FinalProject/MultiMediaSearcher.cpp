#include "MultiMediaSearcher.h"

MultiMediaSearcher::MultiMediaSearcher() {
	queryVideo = "";
	queryAudio = "";

	result = "";
	index = -1;
}

void MultiMediaSearcher::search() {
	namedWindow("FrameStartRef", 1);
	/*
	printf("Start Load Src\n");
	VideoHandler videoObjSrc = VideoHandler::VideoHandler();
	videoObjSrc.setFileName("Data/Videos/video11.mp4");
	videoObjSrc.processFrames();
	DominantColorList Signal = videoObjSrc.getProcessedData();
	printf("End Load Src\n");
	Signal.dumpData("PreprocessData/video11");
	return;
	//*/
	
	//*
	printf("Start Load Query\n");
	string queryFilePath = "Data/Queries/";
	string query = "video1_1"; 

	VideoHandler videoObj = VideoHandler::VideoHandler(); 
	videoObj.setFileName(queryFilePath + query + ".mp4");
	videoObj.processFrames(); 
	
	DominantColorList subsignal = videoObj.getProcessedData(); 
	subsignal.setSrcVideo(queryFilePath + "audios/video" + query + ".wav");
	subsignal.findFirstFrame(0); 


	printf("End Load Query\n"); 
	//subsignal.dumpData("PreprocessData/test_query");
	//*/

	/**
	printf("\n\n----------------------------------------------------------\n\n");
	string filePath = "PreprocessData/video";
	string fileSrc = "Data/Videos/video";
	//for (int i = 1; i < 12; ++i) {
		int i = 1;
		DominantColorList signal = DominantColorList();
		string f = filePath + std::to_string(i);
		signal.populateData(f);
		signal.setSrcVideo(fileSrc + std::to_string(i) + ".mp4");


		int test = signal.containsSubset(&subsignal);
		printf("RESULT of %d: %d\n", i, test);

		if (test != -1) {
			result = "Data/Videos/video" + std::to_string(i) + ".mp4";
			index = test;
			VideoCapture capture(result);
			Mat frame;
			capture.set(CAP_PROP_POS_FRAMES, test);
			capture >> frame;
			imshow("FrameStartRef", frame);
			waitKey();
		}
	//}
	//*/
}
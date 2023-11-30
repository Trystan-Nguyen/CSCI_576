#include "MultiMediaSearcher.h"

MultiMediaSearcher::MultiMediaSearcher() {
	queryVideo = "";
	queryAudio = "";

	result = "";
	index = -1;
	totalFrames = 0;
}

void MultiMediaSearcher::search() {
	VideoHandler videoObj = VideoHandler::VideoHandler();
	videoObj.setFileName(queryVideo);
	videoObj.processFrames();
	DominantColorList subVideo = videoObj.getProcessedData();

	AudioHandler subAudio = AudioHandler::AudioHandler();
	subAudio.setFilePath(queryAudio);
	subAudio.processAudio();


	string preprocessPath = "PreprocessData/video";
	string srcVideoPath = "Data/Videos/video";

	for (int i = 1; i < 12; ++i) {
	//int i = 6;
		string f = preprocessPath + std::to_string(i);

		DominantColorList srcVideo = DominantColorList();
		srcVideo.setSrcVideo(srcVideoPath + std::to_string(i) + ".mp4");
		srcVideo.populateDataHue(f);

		AudioHandler srcAudio = AudioHandler::AudioHandler();
		srcAudio.populateData(f);

		if (srcVideo.checkHueSpectrum(&subVideo)) {
			srcVideo.populateDataVec(f);
			//if (srcVideo.getNumUnusedHues() > 200) {
			if (i==1 || i==6 || i==9 || i ==10) {
				int temp_index = srcAudio.compareAudio(&subAudio);
				if (srcVideo.acceptableColorIndex(temp_index, &subVideo)){
					index = temp_index;
					totalFrames = srcVideo.getHueVec()->size();
					result = srcVideoPath + to_string(i) + ".mp4";
				
					firstFrame = srcVideo.findFirstFrame(index);

					return;
				}
			}
			else {
				int temp_index = srcVideo.containsSubset(&subVideo);
				if (temp_index != -1) {
					index = temp_index;
					totalFrames = srcVideo.getHueVec()->size();
					result = srcVideoPath + to_string(i) + ".mp4";

					firstFrame = srcVideo.findFirstFrame(index);

					return;
				}
			}
		}
	}//
}

	


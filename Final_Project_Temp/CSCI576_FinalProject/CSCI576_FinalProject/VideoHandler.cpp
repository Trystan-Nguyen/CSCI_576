#include "VideoHandler.h"


VideoHandler::VideoHandler() {
	filename = "";
	scenes = {};
}

int rgbToHue(int _red, int _blue, int _green) {
	double b = double(_blue) / 255;
	double g = double(_green) / 255;
	double r = double(_red) / 255;

	double maxColor = max(b, max(g, r));
	double minColor = min(b, min(g, r));
	double c = maxColor - minColor;

	double h = 0;
	if (c <= 0)
		h = 0;
	else if (r == maxColor) {
		double segment = (g - b) / c;
		h = segment + ((b > g) ? 6.0 : 0.0);
	}
	else if (g == maxColor) {
		double segment = (b - r) / c;
		h = segment + 2;
	}
	else if (b == maxColor) {
		double segment = (r - g) / c;
		h = segment + 4;
	}

	int hue = round(h * 60);
	if (hue == 360) hue = 0;

	return hue;
}

void VideoHandler::processFrames() {
	VideoCapture capture(filename);
	Mat frame;
	if (!capture.isOpened())
		throw "Error when reading video";
	
	scenes.push_back(SceneData::SceneData());
	scenes.back().setStartFrameIndex(0);
	int* prevImg = nullptr;

	// TEMP-----------------------
	int tempMax = 0;
	namedWindow("w", 1);

    for (; ; ){
        capture >> frame;
        if (frame.empty())
            break;

		int* currentImg = new int[Width*Height/4];
		for (int i = 0; i < Width*Height/4; ++i) currentImg[i] = 0;
		int totalDeviation = 0;

		for (int r = 1; r < frame.rows; r+=3) {
			for (int c = 1; c < frame.cols; c+=3) {
				Vec3b BGR_pixel = frame.at<Vec3b>(r, c);
				currentImg[r/2 * Height/2 + c/2] = rgbToHue(BGR_pixel[2], BGR_pixel[1], BGR_pixel[0]);
				
				if (prevImg != nullptr) {
					int minDeviation = maxDeviation+1;
					for (int rr = r - 1; rr < r + 2; ++rr) {
						for (int cc = c - 1; cc < c + 2; ++cc) {
							Vec3b current_BGR_pixel = frame.at<Vec3b>(rr, cc);
							int currentHue = rgbToHue(BGR_pixel[2], BGR_pixel[1], BGR_pixel[0]);
							int subDeviation = min(
								abs(prevImg[r / 2 * Height / 2 + c / 2] - currentHue),
								360 - abs(prevImg[r / 2 * Height / 2 + c / 2] - currentHue));
							minDeviation = min(minDeviation, subDeviation);
						}
					}

					totalDeviation += minDeviation;
				}
			}
		}

		tempMax = max(totalDeviation, tempMax);

		if (prevImg == nullptr) {
			scenes.back().addFrame(currentImg); 
			prevImg = currentImg;

			// TEMP-----------------------
			imshow("w", frame);
			waitKey(20); // waits to display frame
			scenes.back().setFirstFrame(frame.clone());
		}
		else {
			/*
			int totalDeviation = 0;
			for (int i = 0; i < Width*Height; ++i) {
				totalDeviation += min(abs(prevImg[i] - currentImg[i]), 360 - abs(prevImg[i] - currentImg[i]));
			}

			tempMax = max(totalDeviation, tempMax);
			*/
			if (totalDeviation > maxDeviation) {
				int startIndex = scenes.back().getFrameCount() + scenes.back().getStartFrameIndex();
				scenes.push_back(SceneData::SceneData());
				scenes.back().setStartFrameIndex(startIndex);
				
				// TEMP-----------------------
				imshow("w", frame);
				waitKey(20); // waits to display frame
				scenes.back().setFirstFrame(frame.clone());
				printf("Causing Deviation: %d\n", totalDeviation);
			}
			scenes.back().addFrame(currentImg);
			

			delete[] prevImg;
			prevImg = currentImg;
		}
    }
	// TEMP-----------------------
	printf("---------------------------------------------------\nMAX DEVIATION: %d\n", tempMax);
}

void VideoHandler::collapseShortClips() {
	std::list<SceneData> newScenes = {};
	for (SceneData i : scenes) {
		if (newScenes.empty()) newScenes.push_back(i);
		else if (i.getFrameCount() < 5) {
			newScenes.back().mergeSceneData(i);
		}
		else newScenes.push_back(i);
	}

	scenes = newScenes;
}
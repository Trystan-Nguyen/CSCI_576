#include "DominantColorList.h"

int rgbToHue(int _red, int _green, int _blue) {
	//return 0.257 * _red + 0.504 * _green + 0.098 * _blue + 16;
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

DominantColorList::DominantColorList() {
	validHues = {};
	dominantHues = new int[360];
	for (int i = 0; i < 360; ++i) dominantHues[i] = 0;
}

void DominantColorList::addDominantHue(vector<tuple<int, int>> dominantColors) {
	int* colors = new int[dominantColors.size()];
	for (int i = 0; i < dominantColors.size(); ++i) {
		tuple<int, int> d = dominantColors.at(i);
		++dominantHues[get<0>(d)];
		colors[i] = get<0>(d);
	}
	validHues.push_back(colors);
}

bool isValidHue(int* colorSet1, int* colorSet2) {
	int colorDiff = 360;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			int diff = abs(colorSet1[i] - colorSet2[j]);
			diff = min(diff, 360 - diff);
			colorDiff = min(colorDiff, diff);
		}
	}
	return colorDiff == 0;
}

int DominantColorList::checkHueSpectrum() {
	int numZeros = 0;
	for (int i = 0; i < 360; ++i) {
		if (dominantHues[i] == 0) 
			++numZeros;
	}
	return numZeros;
}

int DominantColorList::containsSubset(DominantColorList* subsample) {
	int* subSampleDominantHues = subsample->getHueInformation();
	for (int i = 0; i < 360; ++i) {
		if (subSampleDominantHues[i] != 0 && dominantHues[i] == 0) {
			//printf("\n\nEND EARLY at %d\n\n", i);
			return -1;
		}
	}

	vector<int*> subSignal = *subsample->getHueVec();
	for (int i = 0; i < validHues.size() - subSignal.size() + 1; ++i) {
		/*
		if (i == 13350) {
			printf("\t%d %d %d\n", validHues[i][0], validHues[i][1], validHues[i][2]);
			printf("\t%d %d %d\n\n", subSignal.front()[0], subSignal.front()[1], subSignal.front()[2]);
		}
		*/

		if (isValidHue(validHues[i], subSignal.front()) && 
			isValidHue(validHues[i + subSignal.size() - 1], subSignal.back())) {
			//printf("CHECK ENTRY: %d\n\n", i);
			
			if (checkIfValidSubset(i, &subSignal)) {
				findFirstFrame(i);
				//if (compareFrames(subsample->getFirstFrame()))
					return i;
			}
		}
	}
	return -1;
}

bool DominantColorList::checkIfValidSubset(int offset, vector<int*>* subSignal) {
	for (int i = 0; i < subSignal->size(); ++i) {
		if (!isValidHue(validHues[offset + i], (*subSignal)[i])) {
			/*
			if (offset == 13350) {
				printf("Failed at index: %d\n", i);
				printf("\t%d %d %d\n", validHues[offset + i][0], validHues[offset + i][1], validHues[offset + i][2]);
				printf("\t%d %d %d\n\n", (*subSignal)[i][0], (*subSignal)[i][1], (*subSignal)[i][2]);
			}
			*/
			return false;
		}
	}
	return true;
}

void DominantColorList::dumpData(string filepath) {
	std::ofstream of1(filepath + ".hue");
	for (int count = 0; count < 360; count++) {
		of1 << dominantHues[count] << "\n";
	}
	of1.close();

	std::ofstream of2(filepath + ".vec");
	for (int* colors : validHues) {
		of2 << colors[0] << " " << colors[1] << " " << colors[2] << "\n";
	}
	of2.close();
}
void DominantColorList::populateData(string filepath) {
	std::ifstream if1(filepath + ".hue");
	int hueExist;
	int i = 0;
	while(if1 >> hueExist){
		dominantHues[i++] = hueExist;
	}
	if1.close();
	
	std::ifstream if2(filepath + ".vec");
	int a; int b; int c;
	while (if2 >> a >> b >> c){
		int* colors = new int[3];
		colors[0] = a;
		colors[1] = b;
		colors[2] = c;
		validHues.push_back(colors);
	}
	if2.close();
}

void DominantColorList::findFirstFrame(int i) {
	VideoCapture capture(srcVideo);
	Mat frame;
	capture.set(CAP_PROP_POS_FRAMES, i);
	capture >> frame;
	firstFrame = frame.clone();
}

Mat DominantColorList::getFirstFrame() {
	return firstFrame;
}

bool DominantColorList::compareFrames(Mat frameCmp) {
	
	for (int r = 1; r < firstFrame.rows; r+=5) {
		for (int c = 1; c < firstFrame.cols; c+=5) {
			Vec3b px1 = firstFrame.at<Vec3b>(r, c);
			int h1 = rgbToHue(px1[2], px1[1], px1[0]);
			Vec3b px2 = frameCmp.at<Vec3b>(r, c);
			int h2 = rgbToHue(px2[2], px2[1], px2[0]);
			int hueDiff = abs(h1 - h2);
			if (min(hueDiff, 360 - hueDiff) > 5) return false;
		}
	}

	return true;
}

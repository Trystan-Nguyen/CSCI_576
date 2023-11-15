#include "VideoHandler.h"


VideoHandler::VideoHandler() {
	filename = "";
	data = DominantColorList();
}

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

void VideoHandler::processFrames() {
	VideoCapture capture(filename);
	Mat frame;
	if (!capture.isOpened())
		throw "Error when reading video";

    for (; ; ){
        capture >> frame;
        if (frame.empty())
            break;
		
		int* hueHist = new int[360];
		int maxHue = -1;
		int maxHueOccurence = -1;
		for (int i = 0; i < 360; ++i) hueHist[i] = 0;
		for (int r = 1; r < frame.rows; ++r) {
			for (int c = 1; c < frame.cols; ++c) {
				Vec3b BGR_pixel = frame.at<Vec3b>(r, c);
				int hue = rgbToHue(BGR_pixel[2], BGR_pixel[1], BGR_pixel[0]);
				++hueHist[hue];
				if (hueHist[hue] > maxHueOccurence) {
					maxHue = hue;
					maxHueOccurence = hueHist[hue];
				}
			}
		}

		data.addDominantHue(maxHue, maxHueOccurence);
		delete[] hueHist;
    }
	
}

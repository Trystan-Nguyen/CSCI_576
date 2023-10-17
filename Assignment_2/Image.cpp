//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"

// Constructor and Desctructors
MyImage::MyImage()
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
	numObjs = 0;

	objsHistograms = NULL;

}

MyImage::~MyImage()
{
	if (Data)
		delete Data;

	if (objsHistograms)
		delete objsHistograms;
}


// Copy constructor
MyImage::MyImage(MyImage* otherImage)
{
	Height = otherImage->Height;
	Width = otherImage->Width;
	Data = new char[Width * Height * 3];
	strcpy(otherImage->ImagePath, ImagePath);

	for (int i = 0; i < (Height * Width * 3); i++)
	{
		Data[i] = otherImage->Data[i];
	}


}



// = operator overload
MyImage& MyImage::operator= (const MyImage& otherImage)
{
	Height = otherImage.Height;
	Width = otherImage.Width;
	Data = new char[Width * Height * 3];
	strcpy((char*)otherImage.ImagePath, ImagePath);

	for (int i = 0; i < (Height * Width * 3); i++)
	{
		Data[i] = otherImage.Data[i];
	}

	return *this;

}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0)
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}

	// Create a valid output file pointer
	FILE* IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if (IN_FILE == NULL)
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char* Rbuf = new char[Height * Width];
	char* Gbuf = new char[Height * Width];
	char* Bbuf = new char[Height * Width];

	for (i = 0; i < Width * Height; i++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width * Height; i++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width * Height; i++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}

	// Allocate Data structure and copy
	Data = new char[Width * Height * 3];
	for (i = 0; i < Height * Width; i++)
	{
		Data[3 * i] = Bbuf[i];
		Data[3 * i + 1] = Gbuf[i];
		Data[3 * i + 2] = Rbuf[i];
	}

	// Clean up and return
	delete[] Rbuf;
	delete[] Gbuf;
	delete[] Bbuf;
	fclose(IN_FILE);

	return true;

}



// MyImage functions defined here
bool MyImage::WriteImage()
{
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0)
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}

	// Create a valid output file pointer
	FILE* OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if (OUT_FILE == NULL)
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char* Rbuf = new char[Height * Width];
	char* Gbuf = new char[Height * Width];
	char* Bbuf = new char[Height * Width];

	for (i = 0; i < Height * Width; i++)
	{
		Bbuf[i] = Data[3 * i];
		Gbuf[i] = Data[3 * i + 1];
		Rbuf[i] = Data[3 * i + 2];
	}


	// Write data to file
	for (i = 0; i < Width * Height; i++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width * Height; i++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width * Height; i++)
	{
		fputc(Bbuf[i], OUT_FILE);
	}

	// Clean up and return
	delete[] Rbuf;
	delete[] Gbuf;
	delete[] Bbuf;
	fclose(OUT_FILE);

	return true;

}

void MyImage::setImagePath(const char* path) {
	strcpy(ImagePath, path);

	int modPtr = 0;
	for (int i = 0; ImagePath[i] != 0; ++i) {
		if (ImagePath[i] == '\\' && ImagePath[i + 1] != 0) modPtr = i + 1;
	}
	fileName = ImagePath + modPtr;
}


struct HSL {
	int hue;
	int sat;
	int light;
};
HSL rgbToHSL(int _red, int _blue, int _green) {
	double b = double(_blue) / 255;
	double g = double(_green) / 255;
	double r = double(_red) / 255;

	double max = max(b, max(g, r));
	double min = min(b, min(g, r));
	double c = max - min;

	double h = 0;
	if (c <= 0)
		h = 0;
	else if (r == max) {
		double segment = (g - b) / c;
		h = segment + ((b > g) ? 6.0 : 0.0);
	}
	else if (g == max) {
		double segment = (b - r) / c;
		h = segment + 2;
	}
	else if (b == max) {
		double segment = (r - g) / c;
		h = segment + 4;
	}

	int hue = round(h * 60);
	if (hue == 360) hue = 0;
	if (hue < 0 || hue >= 360) {
		//printf("RED: %d\t\tGREEN: %d\tBLUE: %d\n", _red, _green, _blue);
		hue = -1;
	}

	int sat = 0;
	if (c != 0) sat = round(100 * double(c) / (1 - abs(int((max + min) - 1))));

	int light = round(double(max - min) * 100 / 2);
	return { hue, sat, light };
}

void MyImage::setNumObj(int i) {
	numObjs = i;
	objsHistograms = new histograms[numObjs];
	objIndex = 0;
}

bool MyImage::checkSurroundingPixels(int index) {
	int curW = index % (Width * 3);
	int curH = round(double(index) / (Width * 3));

	int val = 0;
	int count = 0;
	for (int h = curH - 1; h <= curH + 1; ++h) {
		for (int w = curW - 3; w <= curW + 3; w += 3) {
			if (h < 0 || w < 0 || h >= Height || w >= (Width * 3)) continue;
			else if (h == curH && w == curW) continue;

			int i = h * Width * 3 + w;
			int _red = static_cast<int>(Data[i + 2] & 0xFF);
			int _blue = static_cast<int>(Data[i + 0] & 0xFF);
			int _green = static_cast<int>(Data[i + 1] & 0xFF);
			int hue = rgbToHSL(_red, _blue, _green).hue;

			if (hue == 120 || hue == -1) {
				++val;
			}
			++count;
		}
	}
	//printf("%d\t%d\n", val, count);
	return val > 0;
}

unsigned int* MyImage::buildHistogram() {
	unsigned int* hueHist = new unsigned int[362];
	range* satHist = new range[360];

	for (int i = 0; i < 362; ++i) hueHist[i] = 0;
	for (int i = 0; i < 360; ++i) satHist[i] = { -1,-1 };

	for (int i = 0; i < Height * Width * 3; i += 3) {
		int _red = static_cast<int>(Data[i + 2] & 0xFF);
		int _blue = static_cast<int>(Data[i + 0] & 0xFF);
		int _green = static_cast<int>(Data[i + 1] & 0xFF);

		HSL color = rgbToHSL(_red, _blue, _green);
		int hue = color.hue;
		int sat = color.sat;
		int light = color.light;

		// Skip Green Screen
		if (hue == 120 || hue == -1) continue;
		else if (checkSurroundingPixels(i)) continue;
		else if (light < 5) {
			hueHist[360] = 1;
			continue;
		}
		else if (light > 95) {
			hueHist[361] = 1;
			continue;
		}

		++hueHist[hue];
		if (satHist[hue].max == -1 && satHist[hue].min == -1) satHist[hue] = { sat, sat };
		else if (sat > satHist[hue].max) satHist[hue].max = sat;
		else if (sat < satHist[hue].min) satHist[hue].min = sat;
	}

	char* name = new char[strlen(fileName)];
	strcpy(name, fileName);
	histograms ret = { hueHist, satHist, name };
	

	objsHistograms[objIndex] = ret;
	++objIndex;

	return hueHist;
}

void MyImage::saveHist(char* name, unsigned int* hist) {
	FILE* OUT_FILE;
	OUT_FILE = fopen(name, "wb");

	for (int i = 0; i < 360; ++i) {
		fprintf(OUT_FILE, "%d\n", hist[i]);
	}

	fclose(OUT_FILE);
}

int chunkSize = 10;
void MyImage::bfs(char* pixelData, int index, clusterData* clusterPtr) {
	pixelData[index] = 0;
	int curW = index % Width;
	int curH = round(double(index) / Width);
	++clusterPtr->size;
	clusterPtr->minW = min(clusterPtr->minW, curW);
	clusterPtr->maxW = max(clusterPtr->maxW, curW);
	clusterPtr->minH = min(clusterPtr->minH, curH);
	clusterPtr->maxH = max(clusterPtr->maxH, curH);

	for (int r = curH - chunkSize; r <= curH + chunkSize; ++r) {
		for (int c = curW - chunkSize; c <= curW + chunkSize; ++c) {
			if (r < 0 || c < 0 || r >= Height || c >= Width) continue;
			if (pixelData[r * Width + c] == 1) {
				bfs(pixelData, r * Width + c, clusterPtr);
			}
		}
	}
}


MyImage::detectionFrames* MyImage::clusteringFunction(char* pixelData, char* name) {
	clusterData** arr = new clusterData * [250];
	for (int i = 0; i < 250; ++i) arr[i] = NULL;

	int clusterId = 0;

	for (int i = 0; i < Width * Height; ++i) {
		if (pixelData[i] == 1) {
			clusterData* clusterPtr = new MyImage::clusterData();
			clusterPtr->minW = i % Width;
			clusterPtr->maxW = i % Width;
			clusterPtr->minH = round(double(i) / Width);
			clusterPtr->maxH = round(double(i) / Width);
			clusterPtr->size = 0;
			bfs(pixelData, i, clusterPtr);

			//printf("Cluster Size: %d\n", clusterPtr->size);
			//printf("\t %d %d %d %d\n", cluster.minW, cluster.maxW, cluster.minH, cluster.maxH);
			if (clusterPtr->size > 100) {
				arr[clusterId++] = clusterPtr;
			}
			else {
				delete clusterPtr;
			}
		}
	}
	clusterData* cleanArr = new clusterData[clusterId];
	for (int i = 0; i < clusterId; ++i) {
		cleanArr[i] = *arr[i];
		delete arr[i];
	}
	delete[] arr;
	MyImage::detectionFrames* ret = new MyImage::detectionFrames({ cleanArr, clusterId, name });
	return ret;
}

int filterSize = 5;
char* avgFilter(char* pixels, int width, int height, double comp) {
	char* avgData = new char[width * height];
	for (int h = 0; h < height; ++h) {
		for (int w = 0; w < width; ++w) {
			double pixel = 0;
			int count = 0;
			for (int dh = h - filterSize; dh <= h + filterSize; ++dh) {
				for (int dw = w - filterSize; dw <= w + filterSize; ++dw) {
					if (dh < 0 || dw < 0 || dh >= height || dw >= width) continue;
					pixel += pixels[dh * width + dw];
					++count;
				}
			}
			avgData[h * width + w] = (pixel / count) > comp;
		}
	}
	delete[] pixels;
	return avgData;
}

bool printMe = false;
MyImage::detectionFrames* MyImage::objDetect(int histIndex) {
	unsigned int* histHueOG = objsHistograms[histIndex].hueHist;
	unsigned int* histHue = new unsigned int[362];
	range* histSat = objsHistograms[histIndex].satHist;

	for (int i = 0; i < 360; ++i) histHue[i] = 0;
	histHue[360] = histHueOG[360];
	histHue[361] = histHueOG[361];
	for (int i = 0; i < 360; ++i) {
		for (int j = -5; j <= 5; ++j) {
			int h = i + j;
			if (h < 0) h += 360;
			else if (h > 359) h -= 360;
			histHue[h] += histHueOG[i];
		}
	}

	double avgHue = 0;
	int greenPresenceCount = 0;
	int greenPeakFrequency = 0;
	int bins = 0;
	int relevantBins = 0;
	for (int i = 0; i < 360; ++i) {
		if (histHue[i] > 0) {
			avgHue += histHueOG[i];
			++bins;
			if (histHueOG[i] > 25) ++relevantBins;

			if (i > 75 && i < 140 && histSat[i].max > 15) {
				++greenPresenceCount;
				greenPeakFrequency = max(greenPeakFrequency, histHueOG[i]);
			}
		}
	}
	avgHue = round(avgHue / bins);

	bool checkForGreen = greenPresenceCount > 50 && greenPeakFrequency > 25 && greenPeakFrequency < avgHue;
	//printf("Green Count: %d \t GreenFreq: %d\n", greenPresenceCount, greenPeakFrequency);

	char* idealPixels = new char[Width * Height];

	for (int i = 0; i < Width * Height * 3; i += 3) {
		HSL color = rgbToHSL(
			static_cast<int>(Data[i + 2] & 0xFF),
			static_cast<int>(Data[i + 0] & 0xFF),
			static_cast<int>(Data[i + 1] & 0xFF));
		int hue = color.hue;
		int sat = color.sat;
		int light = color.light;

		if (light < 10 && histHue[360] == 1) idealPixels[i / 3] = 0;
		else if (light > 90 && histHue[361] == 1) idealPixels[i / 3] = 0;
		else if (histHueOG[hue] > 0.85 * avgHue && sat >= histSat[hue].min - 15 && sat <= histSat[hue].max + 25)
			idealPixels[i / 3] = 1;
		else idealPixels[i / 3] = 0;
	}

	//printf("%d %d\n", relevantBins, bins);

	if (relevantBins < 25) idealPixels = avgFilter(idealPixels, Width, Height, 0.75);
	if (bins - relevantBins > 50) idealPixels = avgFilter(idealPixels, Width, Height, 0.8);
	for (int x = 0; x < 5; ++x) idealPixels = avgFilter(idealPixels, Width, Height, 0.25);


	//--------------------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------------------
	/**
	for (int i = 0; i < Width * Height * 3; i += 3) {
		if (idealPixels[i / 3] == 0) {
			Data[i + 0] = 120;
			Data[i + 1] = 120;
			Data[i + 2] = 120;
		}
	} return NULL;
	//*/
	//--------------------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------------------

	detectionFrames* clusters = clusteringFunction(idealPixels, objsHistograms[histIndex].name);

	int validClusters = 0;
	for (int i = 0; i < clusters->frameCounts; ++i) {
		if (clusters->frames[i].size < 10) {
			clusters->frames[i].size = 0;
			continue;
		}

		//printMe = i == 5;
		double clusterStatus = compareHistogram(histHueOG, histSat,
			max(clusters->frames[i].minW, 0) + 5,
			max(clusters->frames[i].minH, 0) + 5,
			clusters->frames[i].maxW - 5,
			clusters->frames[i].maxH - 5,
			avgHue, relevantBins, checkForGreen);

		if (clusterStatus > 0.1) {
			//if (!printMe) {
			clusters->frames[i].size = 0;
		}

		//printf("clusterStatus:\t%f\n", clusterStatus);

		/**
		printf("ClusterId: %d\n", i);
		printf("\tSize: %d\n", clusters->frames[i].size);
		printf("\tRanges: %d %d %d %d\n", clusters->frames[i].minH, clusters->frames[i].minW, clusters->frames[i].maxH, clusters->frames[i].maxW);
		printf("\tStatus: %f\n", clusterStatus);
		//*/
		++validClusters;
	}

	delete[] histHue;
	delete[] histHueOG;
	delete[] histSat;
	return clusters;
}

double MyImage::compareHistogram(unsigned int* objHist, range* histSat, int startW, int startH, int endW, int endH, int avg, int bins, boolean checkGreen) {
	unsigned int* hueHist = new unsigned int[363];
	for (int i = 0; i < 363; ++i) hueHist[i] = 0;

	int refHueRange = 1;
	int refSatRange = 2;
	if (bins > 25) {
		refHueRange = 5;
		refSatRange = 15;
	}

	for (int h = startH; h < endH && h < Height; ++h) {
		for (int w = startW; w < endW && w < Width; ++w) {
			int location = h * 3 * Width + w * 3;
			HSL color = rgbToHSL(
				static_cast<int>(Data[location + 2] & 0xFF),
				static_cast<int>(Data[location + 0] & 0xFF),
				static_cast<int>(Data[location + 1] & 0xFF));
			int hue = color.hue;
			int sat = color.sat;
			int light = color.light;
			if (light < 5) ++hueHist[360];
			else if (light > 95) ++hueHist[361];
			else {
				if (hue > 75 && hue < 140 && sat > 15) hueHist[362] = 1;
				for (int j = -refHueRange; j <= refHueRange; ++j) {
					int h = hue + j;
					if (h < 0) h += 360;
					else if (h > 359) h -= 360;
					if (sat >= histSat[h].min - refSatRange && sat <= histSat[h].max + refSatRange)
						++hueHist[h];
				}
			}

			/**
			if (printMe) {
				Data[location] = 0;
				Data[location + 1] = 0;
				Data[location + 2] = 0;
			}
			//*/
		}
	}
	/**
	if (printMe) {
		for (int i = 0; i < 360; ++i) {
			//printf("%d %d\n", i , hueHist[i]);
			printf("%d (%d) [%d - %d]\n", i, hueHist[i], histSat[i].min, histSat[i].max);
		}
	}
	//*/

	double missingVals = 0;
	int count = 0;
	for (int i = 0; i < 360; ++i) {
		if (checkGreen && i > 75 && i < 140) {
			if (printMe) printf("SKIPING_GREEN----------------\n");
			if (hueHist[362] == 0) {
				++missingVals;
				if (printMe) printf("\tFAILED\n");
			}
			++count;
		}
		else if (bins > 25 && i >= 60 && i <= 120 && objHist[i] < 0.01 * avg) {
			//printf("CHECKHERE----------------\n");
			continue;
		}
		else if (objHist[i] > 15) {
			if (hueHist[i] == 0) {
				++missingVals;
				if (printMe) printf("%d [%d - %d]\n", i, histSat[i].min, histSat[i].max);
			}
			++count;
		}
	}
	if (hueHist[360] == 1) {
		if (objHist[360] == 0) {
			missingVals += 25;
			if (printMe) printf("Black\n");
		}
		++count;
	}
	if (hueHist[361] == 1) {
		if (objHist[361] == 0) {
			missingVals += 25;
			if (printMe) printf("White\n");
		}
		++count;
	}

	if (printMe) printf("%f / %d\n", missingVals, count);

	delete[] hueHist;
	return double(missingVals) / count;
}

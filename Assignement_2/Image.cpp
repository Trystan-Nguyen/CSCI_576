//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"

struct histograms {
	unsigned int* hueHist;
	unsigned int* satHist;
};

// Constructor and Desctructors
MyImage::MyImage() 
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
	numObjs = 0;

	objsHistograms = NULL;
	inputHistogram = NULL;
}

MyImage::~MyImage()
{
	if ( Data )
		delete Data;

	if (objsHistograms)
		delete objsHistograms;

	if (inputHistogram)
		delete inputHistogram;
}


// Copy constructor
MyImage::MyImage( MyImage *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	Data   = new char[Width*Height*3];
	strcpy(otherImage->ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage->Data[i];
	}


}



// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	Height = otherImage.Height;
	Width  = otherImage.Width;
	Data   = new char[Width*Height*3];
	strcpy( (char *)otherImage.ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage.Data[i];
	}
	
	return *this;

}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Width*Height; i ++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}
	
	// Allocate Data structure and copy
	Data = new char[Width*Height*3];
	for (i = 0; i < Height*Width; i++)
	{
		Data[3*i]	= Bbuf[i];
		Data[3*i+1]	= Gbuf[i];
		Data[3*i+2]	= Rbuf[i];
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
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if ( OUT_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Height*Width; i++)
	{
		Bbuf[i] = Data[3*i];
		Gbuf[i] = Data[3*i+1];
		Rbuf[i] = Data[3*i+2];
	}

	
	// Write data to file
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
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
		if (ImagePath[i] == '/' && ImagePath[i + 1] != 0) modPtr = i+1;
	}
	fileName = ImagePath + modPtr;

}


// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify()
{

	








	return false;
}


int rgbToHue(int _red, int _blue, int _green) {
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
		printf("RED: %d\t\tGREEN: %d\tBLUE: %d\n", _red, _green, _blue);
		return -1;
	}

	return hue;
}

int rgbToSat(int _red, int _blue, int _green) {
	double b = double(_blue) / 255;
	double g = double(_green) / 255;
	double r = double(_red) / 255;

	double max = max(b, max(g, r));
	double min = min(b, min(g, r));
	double c = max - min;

	if (c == 0) return 0;
	else return round(100 * double(c) / (1 - abs(int((max + min) - 1))));
}


void MyImage::setNumObj(int i) {
	numObjs = i;
	objsHistograms = new histograms[numObjs];
	objIndex = 0;
}


unsigned int* MyImage::buildHistogram() {
	unsigned int* hueHist = new unsigned int[360];
	range* satHist = new range[360];

	for (int i = 0; i < 360; ++i) hueHist[i] = 0;
	for (int i = 0; i < 360; ++i) satHist[i] = {-1,-1};

	for (int i = 0; i < Height * Width * 3; i += 3) {

		int _red = static_cast<int>(Data[i + 2] & 0xFF);
		int _blue = static_cast<int>(Data[i + 0] & 0xFF);
		int _green = static_cast<int>(Data[i + 1] & 0xFF);

		int hue = rgbToHue(_red, _blue, _green);
		int sat = rgbToSat(_red, _blue, _green);
		
		// Skip Green Screen
		if (hue==120 || hue == -1) continue;
		for (int i = -10; i < 11; ++i) {
			int h = hue + i;
			if (h < 0) h += 360;
			else if (h > 359) h -= 360;
			++hueHist[h];
			if (satHist[h].max == -1 && satHist[h].min == -1) satHist[h] = { sat, sat };
			else if (sat > satHist[h].max) satHist[h].max = sat;
			else if (sat < satHist[h].min) satHist[h].min = sat;
		}

	}

	histograms ret = { hueHist, satHist, ImagePath };

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

int chunkSize = 5;
void MyImage::bfs(char* pixelData, int index, clusterData* clusterPtr) {
	pixelData[index] = 0;
	int curW = index % Width;
	int curH = round(double(index) / Width);
	++clusterPtr->size;
	clusterPtr->minW = min(clusterPtr->minW, curW);
	clusterPtr->maxW = max(clusterPtr->maxW, curW);
	clusterPtr->minH = min(clusterPtr->minH, curH);
	clusterPtr->maxH = max(clusterPtr->maxH, curH);

	for (int r = curH-chunkSize; r <= curH+chunkSize; ++r) {
		for (int c = curW-chunkSize; c <= curW + chunkSize; ++c) {
			if (r < 0 || c < 0 || r >= Height || c >= Width) continue;
			if (pixelData[r * Width + c] == 1) {
				bfs(pixelData, r * Width + c, clusterPtr);
			}
		}
	}
}


MyImage::detectionFrames* MyImage::clusteringFunction(char* pixelData) {
	clusterData** arr = new clusterData*[250];
	for (int i = 0; i < 250; ++i) arr[i] = NULL;
	
	int clusterId = 0;

	for (int i = 0; i < Width*Height; ++i) {
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
			if (clusterPtr->size > 50) {
				arr[clusterId++] = clusterPtr;
			}
			else {
				delete clusterPtr;
			}
		}
	}
	clusterData* cleanArr = new clusterData[clusterId];
	for (int i = 0; i < clusterId; ++i)cleanArr[i] = *arr[i];
	delete[] arr;
	MyImage::detectionFrames* ret = new MyImage::detectionFrames({ cleanArr, clusterId, fileName });
	return ret;
}



MyImage::detectionFrames* MyImage::objDetect(int histIndex) {
	unsigned int* histHue = objsHistograms[histIndex].hueHist;
	int avgHue = 0;
	int bins = 0;
	for (int i = 0; i < 360; ++i) {
		if (histHue[i] > 0) {
			avgHue += histHue[i];
			++bins;
		}
	}
	avgHue = round(double(avgHue) / bins);

	range* histSat = objsHistograms[histIndex].satHist;
	pixelCluster* pixelLoc = new pixelCluster[Height * Width];
	char* idealPixels = new char[Width * Height];

	int pixelIndex = 0;
	for (int i = 0; i < Height * Width; ++i) pixelLoc[i] = {-1, NULL};

	for (int i = 0; i < Width * Height * 3; i+=3) {
		int hue = rgbToHue(
			static_cast<int>(Data[i + 2] & 0xFF),
			static_cast<int>(Data[i + 0] & 0xFF),
			static_cast<int>(Data[i + 1] & 0xFF));
		int sat = rgbToSat(
			static_cast<int>(Data[i + 2] & 0xFF),
			static_cast<int>(Data[i + 0] & 0xFF),
			static_cast<int>(Data[i + 1] & 0xFF));

		if (histHue[hue] > avgHue*1.5 && sat >= histSat[hue].min && sat <= histSat[hue].max) {
			pixelLoc[pixelIndex++] = { i / 3, NULL, 0.0 };
			idealPixels[i/3] = 1;
		}
		
		else {
			idealPixels[i/3] = 0;
			/**
			Data[i + 0] = 0;
			Data[i + 1] = 0;
			Data[i + 2] = 0;	
			*/
		}
		
		
	}
	//return NULL;
	
	detectionFrames* clusters = clusteringFunction(idealPixels);

	int validClusters = 0;
	for (int i = 0; i < clusters->frameCounts; ++i) {
		if (clusters->frames[i].size < 10) {
			clusters->frames[i].size = 0;
			continue;
		}

		int clusterStatus = compareHistogram(histHue, histSat,
			max(clusters->frames[i].minH, 0),
			max(clusters->frames[i].minW,0),
			clusters->frames[i].maxH,
			clusters->frames[i].maxW, avgHue);
		if (clusterStatus != 0) {
			clusters->frames[i].size = 0;
		}

		///**
		printf("ClusterId: %d\n", i);
		printf("\tSize: %d\n", clusters->frames[i].size);
		printf("\tRanges: %d %d %d %d\n", clusters->frames[i].minH, clusters->frames[i].minW, clusters->frames[i].maxH, clusters->frames[i].maxW);
		printf("\tStatus: %d\n", clusterStatus);
		//*/
		++validClusters;
	}

	return clusters;
}

int MyImage::compareHistogram(unsigned int* objHist, range* histSat, int startW, int startH, int endW, int endH, int threshold) {
	unsigned int* hueHist = new unsigned int[360];
	for (int i = 0; i < 360; ++i) hueHist[i] = 0;

	for (int h = startH; h < endH && h < Height; ++h) {
		for (int w = startW; w < endW && w < Width; ++w) {
			int location = h * 3 * Width + w * 3;
			int hue = rgbToHue(
				static_cast<int>(Data[location + 2] & 0xFF),
				static_cast<int>(Data[location + 0] & 0xFF),
				static_cast<int>(Data[location + 1] & 0xFF));
			int sat = rgbToSat(
				static_cast<int>(Data[location + 2] & 0xFF),
				static_cast<int>(Data[location + 0] & 0xFF),
				static_cast<int>(Data[location + 1] & 0xFF));
			if (sat >= histSat[hue].min && sat <= histSat[hue].max) ++hueHist[hue];
			
			
			/*
			for (int i = -10; i <= 10; i++) {
				int h = hue + i;
				if (h < 0) h += 360;
				else if (h > 359) h -= 360;
				++hueHist[h];
			}
			*/
		}
	}

	int missingVals = 0;
	for (int i = 0; i < 360; ++i) {
		if (objHist[i] > 1.5*threshold && hueHist[i] == 0)
			++missingVals;
			//return false;
	}
	//return true;
	return missingVals;
}

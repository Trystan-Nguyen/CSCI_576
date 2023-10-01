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
	DataEdited = NULL;
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


unsigned int* MyImage::buildHistogram(bool isInputImg) {
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
		if ((!isInputImg && hue==120) || hue == -1) continue;
		++hueHist[hue];
		if (satHist[hue].max == -1 && satHist[hue].min == -1) satHist[hue] = { sat, sat };
		else if (sat > satHist[hue].max) satHist[hue].max = sat;
		else if (sat < satHist[hue].min) satHist[hue].min = sat;
	}

	histograms ret = { hueHist, satHist };

	if (!isInputImg) {
		objsHistograms[objIndex] = ret;
		++objIndex;
	}
	else {
		inputHistogram = hueHist;
	}

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

int maxDist = 50;
void MyImage::clusterNeighbors(pixelCluster* data, int size, int index, int id) {
	int* clusterId = new int(id);
	data[index].clusterLabel = clusterId;

	int curW = data[index].location % Width;
	int curH = round(double(data[index].location) / Width);


	for (int i = 0; i < size; ++i) {
		if (i == index) continue;
		int refW = data[i].location % Width;
		int refH = round(double(data[i].location) / Width);

		double dist = sqrt(pow(curW - refW, 2) + pow(curH - refH, 2));
		if (sqrt(pow(curW - refW, 2) + pow(curH - refH, 2)) <= maxDist) {
			if (data[i].clusterLabel == NULL) {
				data[i].clusterLabel = clusterId;
				data[i].dist = dist;
			}
			else if (*data[i].clusterLabel != *data[index].clusterLabel){
				*data[i].clusterLabel = id;
			}
		}
	}
}

int MyImage::clusteringFunction(pixelCluster* data, int size) {
	int clusterId = 0;

	for (int i = 0; i < size; ++i) {
		///*
		if (data[i].clusterLabel != NULL && data[i].dist > double(maxDist) * 0.95) {
			data[i].dist = 0.0;
			clusterNeighbors(data, size, i, *data[i].clusterLabel);
		}
		else if (data[i].clusterLabel == NULL) clusterNeighbors(data, size, i, clusterId++);
		//*/
		/**
		if(data[i].clusterLabel != NULL) clusterNeighbors(data, size, i, *data[i].clusterLabel);
		else clusterNeighbors(data, size, i, clusterId++);
		*/
	}
	return clusterId;
}

void MyImage::objDetect(int histIndex) {
	unsigned int* histHue = objsHistograms[histIndex].hueHist;
	int avgHue = 0;
	int bins = 0;
	for (int i = 0; i < 360; ++i) {
		if (histHue[i] > 0) {
			avgHue += histHue[i];
			++bins;
		}
	}
	avgHue = round(double(avgHue)*.9 / bins);

	range* histSat = objsHistograms[histIndex].satHist;

	unsigned char* detections = new unsigned char [Height * Width];
	pixelCluster* pixelLoc = new pixelCluster[Height * Width];
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

		if (histHue[hue] > avgHue && sat >= histSat[hue].min && sat <= histSat[hue].max) {
			detections[i/3] = 255;
			pixelLoc[pixelIndex++] = { i / 3, NULL, 0.0};
		}
		else {
			detections[i/3] = 0;
		}
	}

	int totalClusterNum = clusteringFunction(pixelLoc, pixelIndex);

	for (int h = 0; h < Height; ++h) {
		for (int w = 0; w < Width; ++w) {
			Data[h * Width * 3 + w * 3 + 0] = detections[h * Width + w];
			Data[h * Width * 3 + w * 3 + 1] = detections[h * Width + w];
			Data[h * Width * 3 + w * 3 + 2] = detections[h * Width + w];
		}
	}

	int* clusterIds = new int[totalClusterNum];
	
	for (int ii = 0; ii < totalClusterNum; ++ii) clusterIds[ii] = 0;
	for (int ii = 0; ii < pixelIndex; ++ii) {
		++clusterIds[*(pixelLoc[ii].clusterLabel)];
		if (*pixelLoc[ii].clusterLabel == 1) {
			Data[pixelLoc[ii].location*3 + 0] = 0;
			Data[pixelLoc[ii].location*3 + 1] = 0;
			Data[pixelLoc[ii].location*3 + 2] = 255;
		}
	}
	for (int ii = 0; ii < totalClusterNum; ++ii) printf("Cluster: %d\tPixels: %d\n", ii, clusterIds[ii]);
	


}

void MyImage::initDataEdited() {
	if (!DataEdited) DataEdited = new char[Width*Height*3];
	for (int i = 0; i < Width * Height * 3; ++i) DataEdited[i] = Data[i];
}

void MyImage::setDataEdited() {
	if (Data) delete Data;
	Data = DataEdited;
}
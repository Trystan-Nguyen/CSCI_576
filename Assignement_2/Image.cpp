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
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
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
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(OUT_FILE);

	return true;

}




// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify()
{

	








	return false;
}





void MyImage::setNumObj(int i) {
	numObjs = i;
	objsHistograms = new unsigned int*[numObjs];
	objIndex = 0;
}

unsigned int* MyImage::buildHistogram(bool isInputImg) {
	unsigned int* hueHist = new unsigned int[360];
	for (int i = 0; i < 360; ++i) hueHist[i] = 0;

	for (int i = 0; i < Height * Width * 3; i += 3) {
		int _red = static_cast<int>(Data[i + 2] & 0xFF);
		int _blue = static_cast<int>(Data[i + 0] & 0xFF);
		int _green = static_cast<int>(Data[i + 1] & 0xFF);

		double b = double(_blue) / 255;
		double g = double(_green) / 255;
		double r = double(_red) / 255;

		

		// Skip Green Screen
		if (!isInputImg && b == 0 && r == 0 && g == 1) continue;

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
			printf("Position: %d\tHue: %d\n", i, hue);
			continue;
		}
		++hueHist[hue];


	}

	if (!isInputImg) {
		objsHistograms[objIndex] = hueHist;
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
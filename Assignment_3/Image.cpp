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
}

MyImage::~MyImage()
{
	if ( Data )
		delete Data;
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
	int *Rbuf = new int[Height*Width]; 
	int *Gbuf = new int[Height*Width]; 
	int *Bbuf = new int[Height*Width]; 

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
	_red = Rbuf;
	_blue = Bbuf;
	_green = Gbuf;
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
	
	// save references
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;

	fclose(OUT_FILE);

	return true;

}

void MyImage::horizontalDWT() {
	int length = Width/2;
	for (int l = 9; l > levels; --l) {
		int* dwt_r = new int[Width * Height];
		int* dwt_g = new int[Width * Height];
		int* dwt_b = new int[Width * Height];

		for(int h = 0; h < Height; ++h){
			for (int i = 0; i < length; ++i) {
				dwt_r[Width * h + i] = (_red[(Width * h) + (2 * i)] + _red[(Width * h) + (2 * i) + 1])/2;
				dwt_r[Width * h + i + length] = (_red[(Width * h) + (2 * i)] - _red[(Width * h) + (2 * i) + 1])/2;

				dwt_g[Width * h + i] = (_green[(Width * h) + (2 * i)] + _green[(Width * h) + (2 * i) + 1])/2;
				dwt_g[Width * h + i + length] = (_green[(Width * h) + (2 * i)] - _green[(Width * h) + (2 * i) + 1])/2;

				dwt_b[Width * h + i] = (_blue[(Width * h) + (2 * i)] + _blue[(Width * h) + (2 * i) + 1])/2;
				dwt_b[Width * h + i + length] = (_blue[(Width * h) + (2 * i)] - _blue[(Width * h) + (2 * i) + 1])/2;
			}
		}
		delete[] _red;
		delete[] _green;
		delete[] _blue;

		_red = dwt_r;
		_green = dwt_g;
		_blue = dwt_b;

		length /= 2;
	}
}

void MyImage::verticalDWT() {
	int length = Height / 2;
	for (int l = 9; l > levels; --l) {
		int* dwt_r = new int[Width * Height];
		int* dwt_g = new int[Width * Height];
		int* dwt_b = new int[Width * Height];

		for (int w = 0; w < Width; ++w) {
			for (int i = 0; i < length; ++i) {
				dwt_r[Width * i + w] = (_red[Width * i * 2 + w] + _red[Width * ((i + 1) * 2) + w])/2;
				dwt_r[Width * (i + length) + w] = (_red[Width * i * 2 + w] - _red[Width * ((i + 1) * 2) + w])/2;

				dwt_g[Width * i + w] = (_green[Width * i * 2 + w] + _green[Width * ((i + 1) * 2) + w])/2;
				dwt_g[Width * (i + length) + w] = (_green[Width * i * 2 + w] - _green[Width * ((i + 1) * 2) + w])/2;

				dwt_b[Width * i + w] = (_blue[Width * i * 2 + w] + _blue[Width * ((i + 1) * 2) + w])/2;
				dwt_b[Width * (i + length) + w] = (_blue[Width * i * 2 + w] - _blue[Width * ((i + 1) * 2) + w])/2;
			}
		}

		delete[] _red;
		delete[] _green;
		delete[] _blue;

		_red = dwt_r;
		_green = dwt_g;
		_blue = dwt_b;

		length /= 2;
	}
}

void MyImage::zeroHighPass() {
	int limit = Width / pow(2, 9 - levels);
	for (int h = 0; h < Height; ++h) {
		for (int w = 0; w < Width; ++w) {
			if (h > limit || w > limit) {
				_red[h * Width + w] = 0;
				_green[h * Width + w] = 0;
				_blue[h * Width + w] = 0;
			}
		}
	}

}

void MyImage::reverseVerticalDWT() {
	int length = Height / pow(2, 9 - levels);
	for (int l = 9; l > levels; --l) {
		int* dwt_r = new int[Width * Height];
		int* dwt_g = new int[Width * Height];
		int* dwt_b = new int[Width * Height];

		for (int w = 0; w < Width; ++w) {
			for (int i = 0; i < length; ++i) {
				dwt_r[Width * i + w] = (_red[Width * i * 2 + w] + _red[Width * ((i + 1) * 2) + w])/2;
				dwt_r[Width * (i + length) + w] = (_red[Width * i * 2 + w] - _red[Width * ((i + 1) * 2) + w])/2;

				/**
				dwt_g[Width * i + w] = (_green[Width * i * 2 + w] + _green[Width * ((i + 1) * 2) + w])/2;
				dwt_g[Width * (i + length) + w] = (_green[Width * i * 2 + w] - _green[Width * ((i + 1) * 2) + w])/2;

				dwt_b[Width * i + w] = (_blue[Width * i * 2 + w] + _blue[Width * ((i + 1) * 2) + w])/2;
				dwt_b[Width * (i + length) + w] = (_blue[Width * i * 2 + w] - _blue[Width * ((i + 1) * 2) + w])/2;
				*/
			}
		}

		delete[] _red;
		delete[] _green;
		delete[] _blue;

		_red = dwt_r;
		_green = dwt_g;
		_blue = dwt_b;

		length *= 2;
	}
}

void MyImage::reverseHorizontalDWT() {

}

// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
void MyImage::Modify(){
	/**
	char* lumaImg = new char[Width * Height];
	for (int i = 0; i < Width * Height * 3; i += 3) {
		char R = static_cast<int>(Data[i + 2] & 0xFF);
		char G = static_cast<int>(Data[i + 1] & 0xFF);
		char B = static_cast<int>(Data[i + 0] & 0xFF);
		lumaImg[i/3] = 0.257 * R + 0.504 * G + 0.098 * B + 16;
	}
	*/

	//horizontalDWT();
	verticalDWT();

	zeroHighPass();

	reverseVerticalDWT();
	//reverseHorizontalDWT();


	for (int i = 0; i < Width * Height * 3; i += 3) {
		Data[i + 0] = _blue[i / 3];
		Data[i + 1] = _green[i / 3];
		Data[i + 2] = _red[i / 3];
	}
}
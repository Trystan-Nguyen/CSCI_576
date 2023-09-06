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
	
	ModifiedData = NULL;
	ModWidth = -1;
	ModHeight = -1;
	Scale = -1;
	Aliasing = -1;
	windowOverlay = -1;
	
}

MyImage::~MyImage()
{
	if ( Data )
		delete Data;
	if (ModifiedData)
		delete ModifiedData;
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
	// TO DO by student
	//printf("ARGS:\n\tSCALE: %f\n\tALIASING: %i\n\tWINDOW: %i\n", Scale, Aliasing, windowOverlay);

	

	// Subsample img and update Data, Width, Height
	if (Scale > 0) {
		ModWidth = round(Width * Scale);
		ModHeight = round(Height * Scale);
		ModifiedData = new char[ModWidth * ModHeight * 3];

		for (int row = 0; row < ModHeight; ++row) {
			for (int col = 0; col < ModWidth * 3; col += 3) {
				if (Aliasing == 0) {
					int refCoord = ((row * Width * 3) + col) / Scale;
					ModifiedData[(row * ModWidth * 3) + col + 0] = Data[refCoord + 0];
					ModifiedData[(row * ModWidth * 3) + col + 1] = Data[refCoord + 1];
					ModifiedData[(row * ModWidth * 3) + col + 2] = Data[refCoord + 2];
				}
				// Use filtered Image (Aliasing)
				else {
					long px1 = 0, px2 = 0, px3 = 0;
					int count = 0;

					int sampleSize = round(1 / (2 * Scale));
					for (int i = row; i < row + sampleSize && i < Height; ++i) {
						for (int j = col; j < col + sampleSize*3 && j < Width * 3; j+=3) {
							int refCoord = ((i * Width * 3) + j) / Scale;
							if (i > Height || j > Width * 3 || refCoord > Height * Width * 3) {
								printf("Height: %i \tWidth: %i\n", i, j);
								break;
							}
							px1 += Data[refCoord + 0];
							px2 += Data[refCoord + 1];
							px3 += Data[refCoord + 2];
							++count;
						}
					}
					ModifiedData[(row * ModWidth * 3) + col + 0] = round(px1 / count);
					ModifiedData[(row * ModWidth * 3) + col + 1] = round(px2 / count);
					ModifiedData[(row * ModWidth * 3) + col + 2] = round(px3 / count);
				}
			}
		}


		///*
		Data = ModifiedData;
		Width = ModWidth;
		Height = ModHeight;
		//*/
	}
	// Aliasing w/o scale
	else if (Aliasing == 1) {
		ModWidth = Width;
		ModHeight = Height;
		ModifiedData = new char[ModWidth * ModHeight * 3];

		for (int row = 0; row < ModHeight; ++row) {
			for (int col = 0; col < ModWidth * 3; col += 3) {
				char px1 = 0, px2 = 0, px3 = 0;
				int sampleSize = ceil(1 / Scale);
				for (int i = row; i < i + sampleSize && i < Height; ++i) {
					for (int j = col; col < j + sampleSize && j < Width * 3; j += 3) {
						int refCoord = ((i * Width * 3) + j) / Scale;
						px1 += Data[refCoord + 0];
						px2 += Data[refCoord + 1];
						px3 += Data[refCoord + 2];
					}
				}
				ModifiedData[(row * ModWidth * 3) + col + 0] = round(px1 / pow(Scale, 2));
				ModifiedData[(row * ModWidth * 3) + col + 1] = round(px2 / pow(Scale, 2));
				ModifiedData[(row * ModWidth * 3) + col + 2] = round(px3 / pow(Scale, 2));
			}
		}
	}
	else {
		ModifiedData = Data;
		ModWidth = Width;
		ModHeight = Height;
	}

	return false;

}

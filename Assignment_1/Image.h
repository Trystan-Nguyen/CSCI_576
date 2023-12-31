//*****************************************************************************
//
// Image.h : Defines the class operations on images
//
// Author - Parag Havaldar
// Main Image class structure 
//
//*****************************************************************************

#ifndef IMAGE_DISPLAY
#define IMAGE_DISPLAY

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "afxwin.h"

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>



// Class structure of Image 
// Use to encapsulate an RGB image
class MyImage 
{

private:
	int		Width;					// Width of Image
	int		Height;					// Height of Image
	char	ImagePath[_MAX_PATH];	// Image location
	char*	Data;					// RGB data of the image
	
	char*	OriginalData;
	int		OriginalWidth;
	int		OriginalHeight;
	double   Scale;
	int		Aliasing;
	int		windowOverlay;

public:
	// Constructor
	MyImage();
	// Copy Constructor
	MyImage::MyImage( MyImage *otherImage);
	// Destructor
	~MyImage();

	// operator overload
	MyImage & operator= (const MyImage & otherImage);

	// Reader & Writer functions
	void	setWidth( const int w)  { Width = w; }; 
	void	setHeight(const int h) { Height = h; }; 
	void	setImageData( const char *img ) { Data = (char *)img; };
	void	setImagePath( const char *path) { strcpy(ImagePath, path); }
	int		getWidth() { return Width; };
	int		getHeight() { return Height; };
	char*	getImageData() { return Data; };
	char*	getImagePath() { return ImagePath; }

	int		getOGWidth() { return OriginalWidth; };
	int		getOGHeight() { return OriginalHeight; };
	int		getScale() { return Scale; };
	void	setScale(const char* s) { Scale = strtod(s, NULL); };
	void	setAliasing(const char* s) { Aliasing = atoi(s); };
	void	setWindowOverlay(const char* s) { windowOverlay = atoi(s); };
	int		getWindowOverlay() { return windowOverlay; }
	char* getOriginalData() { return OriginalData; }

	// Input Output operations
	bool	ReadImage();

	void	removeAndInterpolate();
	bool	WriteImage();

	// Modifications
	bool	Modify();
	char*	getOriginalWindowOverlay(char* subImage, int x, int y);

};

#endif //IMAGE_DISPLAY

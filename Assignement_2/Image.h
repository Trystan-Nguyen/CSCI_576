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
	struct pixelCluster {
		int location;
		int* clusterLabel;
		double dist;
	};
	struct range {
		int min;
		int max;
	};
	struct histograms {
		unsigned int* hueHist;
		range* satHist;
	};
	

	int		Width;					// Width of Image
	int		Height;					// Height of Image
	char	ImagePath[_MAX_PATH];	// Image location
	char*	Data;					// RGB data of the image
	char*  DataEdited;					// RGB data of the image
	
	int		numObjs;
	int		objIndex;
	histograms*	objsHistograms;
	unsigned int* inputHistogram;

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

	// Input Output operations
	bool	ReadImage();
	bool	WriteImage();

	// Modifications
	bool	Modify();

	void setNumObj(int i);
	unsigned int* buildHistogram(bool isInputImg);

	void saveHist(char* name, unsigned int* hist);
	void objDetect(int i);

	void initDataEdited();
	void setDataEdited();

	void clusterNeighbors(pixelCluster* data, int size, int index, int id);
	int clusteringFunction(pixelCluster* data, int size);

};

#endif //IMAGE_DISPLAY

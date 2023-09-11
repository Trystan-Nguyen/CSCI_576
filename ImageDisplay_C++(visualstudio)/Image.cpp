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
	
	OriginalData = NULL;
	OriginalWidth = -1;
	OriginalHeight = -1;
	Scale = -1;
	Aliasing = -1;
	windowOverlay = -1;
	
}

MyImage::~MyImage()
{
	if ( Data )
		delete Data;
	if (OriginalData)
		delete OriginalData;
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


int firstPixelVal(int val) {
	if (val % 3 == 0) return val;
	else if (val%3/3>=0.5) return val + 3 -(val % 3);
	else return val - (val % 3);
	
}
// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify()
{
	OriginalData = Data;
	OriginalWidth = Width;
	OriginalHeight = Height;

	// TO DO by student
	//printf("ARGS:\n\tSCALE: %f\n\tALIASING: %i\n\tWINDOW: %i\n", Scale, Aliasing, windowOverlay);
	
	

	// Subsample img and update Data, Width, Height
	if (Scale > 0 && Scale < 1) {
		int ModWidth = round(Width * Scale);
		int ModHeight = round(Height * Scale);
		char* ModifiedData = new char[ModWidth * ModHeight * 3];

		//printf("OldImage:\n\tWidth: %i\n\tHeight: %i\n", Width, Height);
		//printf("NewImage:\n\tWidth: %i\n\tHeight: %i\n", ModWidth, ModHeight);
		
		for (int row = 0; row < ModHeight; ++row) {
			for (int col = 0; col < ModWidth * 3; col += 3) {
				int refCoord = firstPixelVal(round(col / Scale)) + round((row / Scale)) * Width * 3;
				if (Aliasing == 0) {
					ModifiedData[(row * ModWidth * 3) + col + 0] = Data[refCoord + 0];
					ModifiedData[(row * ModWidth * 3) + col + 1] = Data[refCoord + 1];
					ModifiedData[(row * ModWidth * 3) + col + 2] = Data[refCoord + 2];
				}
				else {
					UINT p1 = 0, p2 = 0, p3 = 0;
					int sum = 0;
					
					int schalars[] = {
						1,4,6,4,1,
						4,16,24,16,4,
						6,24,36,24,6,
						4,16,24,16,4,
						1,4,6,4,1
					};
					

					for (int i = -1; i < 2; ++i) {
						int newRow = row + i;
						if (newRow < 0) continue;
						else if (newRow >= ModHeight) break;
						
						for (int j = -1; j < 2; ++j) {
							int newCol = col + j*3;
							if (newCol < 0) continue;
							else if (newCol >= ModWidth*3) break;
							
							int refCoord = firstPixelVal(round(newCol / Scale)) + 
								round((newRow / Scale)) * Width * 3;
							if (refCoord > (Width * Height * 3)) continue;

							int scale = schalars[5*(i+2)+j];
							p1 += UINT(Data[refCoord + 0] & 0xFF) * scale;
							p2 += UINT(Data[refCoord + 1] & 0xFF) * scale;
							p3 += UINT(Data[refCoord + 2] & 0xFF) * scale;
							sum += scale;
						}
					}
					ModifiedData[(row * ModWidth * 3) + col + 0] = char(round(p1/sum));
					ModifiedData[(row * ModWidth * 3) + col + 1] = char(round(p2/sum));
					ModifiedData[(row * ModWidth * 3) + col + 2] = char(round(p3/sum));
				}
			}
		}

		Data = ModifiedData;
		Width = ModWidth;
		Height = ModHeight;
	}


	return false;

}

char* MyImage::getOriginalWindowOverlay(char* subImage, int x, int y) {
	if (subImage) delete[] subImage;
	//subImage = new char[3 * windowOverlay * windowOverlay];
	subImage = new char[3 * Width * Height];
	
	int newX = round(x / Scale);
	int newY = round(y / Scale);

	int ref = round(windowOverlay / 2);
	for (int i = 0; i < (Height * Width * 3); i++) subImage[i] = Data[i];
	for (int r = -ref; r < ref; ++r) {
		for (int c = -ref; c < ref; ++c) {
			int canvasX = x + c;
			int canvasY = y + r;
			if (canvasX < 0 || canvasY < 0) continue;
			if (canvasX >= Width || canvasY >= Height) continue;
			int canvasCoord = Width * 3 * canvasY + 3*canvasX;

			int ogX = 3*(newX + c);
			int ogY = newY + r;
			if (ogX < 0 || ogY < 0 || ogX >= 3*OriginalWidth || ogY >= OriginalHeight) 
				continue;
			int originalCoord = ogY * OriginalWidth * 3 + ogX;

			subImage[canvasCoord + 0] = OriginalData[originalCoord + 0];
			subImage[canvasCoord + 1] = OriginalData[originalCoord + 1];
			subImage[canvasCoord + 2] = OriginalData[originalCoord + 2];
			
		}
	}

	return subImage;
}

void MyImage::removeAndInterpolate() {
	OriginalData = Data;
	OriginalWidth = Width;
	OriginalHeight = Height;
	
	double errors[51];
	for (int i = 0; i < 51; ++i) errors[i] = 0;

	for (int pS = 1; pS < 51; ++pS) {
		printf("%i\n", pS);
		double percent = pS * 0.01;

		int maxCoords = Width * Height * percent;
		char* ModifiedData = new char[Width * Height * 3];
		for (int i = 0; i < Width * Height * 3; ++i) ModifiedData[i] = Data[i];

		int* coords = new int[round(1.5 * maxCoords)];
		for (int i = 0; i < round(1.5 * maxCoords); ++i) coords[i] = -1;
		int count = 0;

		for (int i = 0; i < Width * Height * 3; i += 3) {
			double random = double(rand()) / RAND_MAX;
			if (random < percent) {
				ModifiedData[i + 0] = 0;
				ModifiedData[i + 1] = 0;
				ModifiedData[i + 2] = 0;
				coords[count] = i;
				++count;
			}
		}

		for (int i = 0; i < count; ++i) {
			int refCoord = coords[i];
			if (coords[i] == -1) break;

			int row = refCoord / (Width * 3);
			int col = refCoord % (Width * 3);

			UINT p0 = 0, p1 = 0, p2 = 0;
			UINT weight = 0;
			for (int r = row - 2; r < row + 3; ++r) {
				for (int c = col - 6; c < col + 9; c += 3) {
					if (r<0 || c<0 || r>Height || c>Width * 3) continue;
					if (r == 0 && c == 0) continue;

					int newCoord = (r * Width * 3) + c;
					if (newCoord > Width * Height * 3) continue;

					if (ModifiedData[newCoord + 0] == 0 &&
						ModifiedData[newCoord + 1] == 0 &&
						ModifiedData[newCoord + 2] == 0)
						continue;

					p0 += ModifiedData[newCoord + 0] & 0xFF;
					p1 += ModifiedData[newCoord + 1] & 0xFF;
					p2 += ModifiedData[newCoord + 2] & 0xFF;
					weight += 1;
				}
			}
			if (weight == 0) {
				ModifiedData[refCoord + 0] = char(round(p0));
				ModifiedData[refCoord + 1] = char(round(p1));
				ModifiedData[refCoord + 2] = char(round(p2));
			}
			else {
				ModifiedData[refCoord + 0] = char(round(p0 / weight));
				ModifiedData[refCoord + 1] = char(round(p1 / weight));
				ModifiedData[refCoord + 2] = char(round(p2 / weight));
			}
			double error = pow(ModifiedData[refCoord + 0] - Data[refCoord + 0], 2) +
				pow(ModifiedData[refCoord + 1] - Data[refCoord + 1], 2) +
				pow(ModifiedData[refCoord + 2] - Data[refCoord + 2], 2);

			errors[pS] += error;

		}




		if (pS == 10 || pS == 25 || pS == 50) {
			Data = ModifiedData;
			char newName[25];
			sprintf(newName, "Part3Data/IMG_%i.rgb", pS);
			strcpy(ImagePath, newName);
			WriteImage();
			Data = OriginalData;
		}
		//Data = ModifiedData;
		delete[] ModifiedData;
	}

	FILE* OUT_FILE;
	OUT_FILE = fopen("Part3Data/IMG_Error.csv", "wb");
	
	for (int i = 0; i < 51; ++i) {
		fprintf(OUT_FILE, "%f,\n", errors[i]);
	}
	
	fclose(OUT_FILE);

}



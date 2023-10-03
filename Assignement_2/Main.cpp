//*****************************************************************************
//
// Main.cpp : Defines the entry point for the application.
// Creates a white RGB image with a black line and displays it.
// Two images are displayed on the screen.
// Left Pane: Input Image, Right Pane: Modified Image
//
// Author - Parag Havaldar
// Code used by students as a starter code to display and modify images
//
//*****************************************************************************


// Include class files
#include "Image.h"
#include <iostream>

#define MAX_LOADSTRING 100

// Global Variables:
MyImage			inImage;						// image objects
HINSTANCE		hInst;							// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int numArgs = 1;
MyImage::clusterData* clusterFrames= new MyImage::clusterData[50];
int clusterFrameIndex = 0;

// Main entry point for a windows application
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// Create a separate console window to display output to stdout
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	// The rest of command line argument is truncated.
	// If you want to use it, please modify the code.
	if (lpCmdLine[0] == 0) {
		wprintf(L"No command line arguement.");
		return -1;
	}

	int pos = 0;
	while (lpCmdLine[pos] != 0) {
		if (lpCmdLine[pos] == ' ') {
			++numArgs;
			lpCmdLine[pos] = 0;
		}
		++pos;
	}
	
	char** argsPtr = new char*[numArgs];
	argsPtr[0] = lpCmdLine;
	int currentArg = 1;
	for (int i = 0; i < pos; ++i) {
		if (lpCmdLine[i] == 0 && i < pos && currentArg < numArgs) {
			argsPtr[currentArg] = lpCmdLine + i + 1;
			++currentArg;
		}
	}
	for (int i = 0; i < numArgs; ++i) {
		printf("%s\n", argsPtr[i]);
	}



	// Set up the images
	int w = 640;
	int h = 480;
	inImage.setWidth(w);
	inImage.setHeight(h);
	
	// Image setups
	inImage.setNumObj(numArgs-1);

	for (int i = 1; i < numArgs; ++i) {
		inImage.setImagePath(argsPtr[i]);
		inImage.ReadImage();
		unsigned int* hueHist = inImage.buildHistogram();

		//char name[25];
		//sprintf(name, "test/args-%i.txt", i);
		//inImage.saveHist(name, hueHist);
	}

	inImage.setImagePath(argsPtr[0]);
	inImage.ReadImage();
	//unsigned int* hueHist = inImage.buildHistogram(true);
	//inImage.saveHist("test/input.txt", hueHist);
	
	for (int i = 0; i < numArgs - 1; ++i) {
		MyImage::detectionFrames* temp = inImage.objDetect(i);
		if (temp == NULL) continue;
		for (int i = 0; i < temp->frameCounts; ++i) {
			//printf("Frame count: %d\t Frame Size: %d\n", i, temp->frames[i].size);
			//printf("\t W: %d %d \tH: %d %d\n", temp->frames->minW, temp->frames->maxW, temp->frames->minH, temp->frames->maxW);
			if (temp->frames[i].size != 0) {
				clusterFrames[clusterFrameIndex++] = *(temp->frames);
			}
		}
	}

	

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IMAGE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_IMAGE);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_IMAGE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_IMAGE;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}


//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   SetWindowPos(hWnd, NULL, 0, 0, inImage.getWidth() + 15, inImage.getHeight() + 60,
	   SWP_NOMOVE);

   return TRUE;
}


//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
HBRUSH frameColor = CreateSolidBrush(RGB(125, 125, 125));
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
// TO DO: part useful to render video frames, may place your own code here in this function
// You are free to change the following code in any way in order to display the video

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	RECT rt;
	GetClientRect(hWnd, &rt);

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case ID_MODIFY_IMAGE:
				   InvalidateRect(hWnd, &rt, false); 
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			{
				hdc = BeginPaint(hWnd, &ps);
				
				/*
				// TO DO: Add any drawing code here...
				char text[1000];
				strcpy(text, "The original image is shown as follows. \n");
				DrawText(hdc, text, strlen(text), &rt, DT_LEFT);
				strcpy(text, "\nUpdate program with your code to modify input image. \n");
				DrawText(hdc, text, strlen(text), &rt, DT_LEFT);
				*/

				BITMAPINFO bmi;
				CBitmap bitmap;
				memset(&bmi,0,sizeof(bmi));
				bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
				bmi.bmiHeader.biWidth = inImage.getWidth();
				bmi.bmiHeader.biHeight = -inImage.getHeight();  // Use negative height.  DIB is top-down.
				bmi.bmiHeader.biPlanes = 1;
				bmi.bmiHeader.biBitCount = 24;
				bmi.bmiHeader.biCompression = BI_RGB;
				bmi.bmiHeader.biSizeImage = inImage.getWidth()*inImage.getHeight();

				SetDIBitsToDevice(hdc,
								  0,0,inImage.getWidth(),inImage.getHeight(),
								  0,0,0,inImage.getHeight(),
								  inImage.getImageData(),&bmi,DIB_RGB_COLORS);

				for (int i = 0; i < clusterFrameIndex; ++i) {
					//const RECT* frame = new RECT({ clusterFrames[i].minW, clusterFrames[i].maxH, clusterFrames[i].maxW, clusterFrames[i].minH });
					const RECT* frameL = new RECT({clusterFrames[i].minW-10, clusterFrames[i].maxH+10, clusterFrames[i].minW-5, clusterFrames[i].minH-10});
					FillRect(hdc, frameL, frameColor);
					const RECT* frameR = new RECT({ clusterFrames[i].maxW+5, clusterFrames[i].maxH+10, clusterFrames[i].maxW+10, clusterFrames[i].minH-10});
					FillRect(hdc, frameR, frameColor);
					const RECT* frameT = new RECT({ clusterFrames[i].minW-10, clusterFrames[i].maxH+10, clusterFrames[i].maxW+10, clusterFrames[i].maxH+5});
					FillRect(hdc, frameT, frameColor);
					const RECT* frameB = new RECT({ clusterFrames[i].minW-10, clusterFrames[i].minH-5, clusterFrames[i].maxW+10, clusterFrames[i].minH-10});
					FillRect(hdc, frameB, frameColor);
					
					//Rectangle(hdc, clusterFrames[i].minW, clusterFrames[i].maxH, clusterFrames[i].maxW, clusterFrames[i].minH);
					//printf("RECT STATUS: %d\n", FillRect(hdc, frame, frameColor));
					//printf("W %d %d\tH %d %d\n", clusterFrames[i].minW, clusterFrames[i].maxW, clusterFrames[i].maxH, clusterFrames[i].minH);
				}

				EndPaint(hWnd, &ps);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}




// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}



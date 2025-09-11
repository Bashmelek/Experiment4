// Experiment4.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Experiment4.h"
#include <memory>

#define MAX_LOADSTRING 100

#define MAX_LOADSTRING 100

const int SEQDEMO_W = 200;
const int SEQDEMO_H = 200; // 168

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawSeqDemo(HWND hWnd);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//my stuff
HBITMAP imageBase;
HBITMAP imageMask;
std::unique_ptr <CreaturePet> thePet;
HWND trueMainWND;
HWND clickWND;

int testcheck = 0;

int globalcounter = 0;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_EXPERIMENT4, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EXPERIMENT4));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EXPERIMENT4));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //(HBRUSH)GetStockObject(HOLLOW_BRUSH); //(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_EXPERIMENT4);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	//new ones below, courtesy of thank you https://stackoverflow.com/questions/4052940/how-to-make-an-opengl-rendering-context-with-transparent-background
	////wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
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
	hInst = hInstance; // Store instance handle in our global variable
	int w = SEQDEMO_W;// 200;
	int h = SEQDEMO_H;// 168;

	thePet = std::make_unique <CreaturePet>();

	//HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_EX_LAYERED | WS_EX_APPWINDOW | WS_EX_TOPMOST | WS_EX_TRANSPARENT,//WS_OVERLAPPEDWINDOW,
	//   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	//thank you Song Zhu and Greg https://stackoverflow.com/questions/65286495/make-a-win32-window-transparent-to-messages-click-touch-keyboard
	HWND hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_APPWINDOW | WS_EX_TOPMOST, szTitle, szWindowClass,//WS_OVERLAPPEDWINDOW,    WS_EX_TRANSPARENT
		WS_VISIBLE | WS_POPUP, 500, 150, w, h, NULL, NULL, hInstance, NULL);

	HWND hWnd2 = CreateWindowEx(WS_EX_LAYERED | WS_EX_APPWINDOW | WS_EX_TOPMOST | WS_EX_TRANSPARENT, szTitle, szWindowClass,//WS_OVERLAPPEDWINDOW,    WS_EX_TRANSPARENT
		WS_VISIBLE | WS_POPUP, 500, 150, w, h, NULL, NULL, hInstance, NULL);

	if (!hWnd || !hWnd2)
	{
		return FALSE;
	}
	trueMainWND = hWnd2;
	clickWND = hWnd;

	//SetCapture(hWnd);
	//ReleaseCapture();
	SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);
	//thank you! https://newbedev.com/creating-a-transparent-window-in-c-win32
	SetLayeredWindowAttributes(hWnd, RGB(124, 254, 124), 0, LWA_COLORKEY);//previously transparent color was RGB(255, 255, 255) white  RGB(124, 254, 124) lime for v6

	//thank you! https://stackoverflow.com/questions/7724000/how-to-hide-collapse-main-menu-in-a-win32-mfc-application
	//Raymond Chen is the man
	SetMenu(hWnd, NULL); /* remove the menu */
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	//section for the clickthrough window
	SetLayeredWindowAttributes(hWnd2, 0, 255, LWA_ALPHA);
	SetLayeredWindowAttributes(hWnd2, RGB(124, 254, 124), 0, LWA_COLORKEY);
	SetMenu(hWnd2, NULL);
	ShowWindow(hWnd2, nCmdShow);
	UpdateWindow(hWnd2);

	return TRUE;
}


//static BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam) {
//	// List visible windows with a non-empty title
//	//PostMessage(hWnd, WM_LBUTTONDOWN, 1, lparam);
//	POINT atp = POINT();
//	atp.x = 500;
//	atp.y = 150;
//	HWND target = ChildWindowFromPoint(hWnd,
//		atp
//	);
//	PostMessage(target, WM_LBUTTONDOWN, 1, lparam);
//	return TRUE;
//}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_TIMER) {
		//return 0;
	}
	switch (message)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 120, NULL);//120 good for demo
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		DrawSeqDemo(hWnd);
	}
	break;
	case WM_TIMER:
	{
		int x = 1;
		x += 1;
		{
			if (hWnd = trueMainWND)
			{
				thePet->AdvanceFrame();
				globalcounter++;
			}
			PostMessage(clickWND, WM_PAINT, wParam, lParam);
			DrawSeqDemo(hWnd);
		}
		break;
	}
	////case WM_MOUSEMOVE:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
		break;
	case WM_LBUTTONDOWN:
	{
		int x = 1;
		x += 1;

		SetWindowPos(hWnd, NULL, -300, 150, SEQDEMO_W, SEQDEMO_H, 0);

		//EnumWindows(enumWindowCallback, lParam);
		//PostMessage(GetDesktopWindow(), message, wParam, lParam);
		break;
	}
	case WM_CANCELMODE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


void GetRotatedDC_DUMB(HDC destDC, HDC sourceDC, int w, int h, double degrees)
{

	HDC tempDC = CreateCompatibleDC(sourceDC);
	//HBITMAP oldtempBMP = (HBITMAP)SelectObject(tempDC, thePet->imageMask);

	/*HDC temp2DC2 = CreateCompatibleDC(sourceDC);
	HBITMAP transparentBMP = (HBITMAP)CreateCompatibleBitmap(sourceDC, w, h);
	HBITMAP oldtempBMP = (HBITMAP)SelectObject(tempDC, tempBMP);*/

	//HBITMAP tempBMP = (HBITMAP)CreateCompatibleBitmap(sourceDC, w, h);

	//thank you demorge at https://stackoverflow.com/questions/7502588/createcompatiblebitmap-and-createdibsection-memory-dcs for 
	//showing me memset and how to set up the binfo right, and using CreateDIBSection
	BITMAPINFO binfo = { 0 };
	//memset(&binfo, 0, sizeof(BITMAPINFO));
	binfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	binfo.bmiHeader.biHeight = h;
	binfo.bmiHeader.biWidth = w;
	binfo.bmiHeader.biSizeImage = sizeof(DWORD) * w * h;
	////binfo.bmiHeader.biSize = w * h;
	binfo.bmiHeader.biPlanes = 1;
	binfo.bmiHeader.biBitCount = sizeof(DWORD) * 8;// 32;
	binfo.bmiHeader.biCompression = BI_RGB;
	binfo.bmiHeader.biXPelsPerMeter = 0;
	binfo.bmiHeader.biYPelsPerMeter = 0;
	binfo.bmiHeader.biClrUsed = 0;
	binfo.bmiHeader.biClrImportant = 0;
	COLORREF *buffer = NULL;
	HBITMAP tempBMP = CreateDIBSection(
		sourceDC,//[in]  HDC              hdc,
		&binfo,//[in]  const BITMAPINFO *pbmi,
		DIB_RGB_COLORS,//[in]  UINT             usage,
		(void**)&buffer,//[out] VOID             **ppvBits,
		NULL, //[in]  HANDLE           hSection,
		0//[in]  DWORD            offset
	);

	HBITMAP oldtempBMP = (HBITMAP)SelectObject(tempDC, tempBMP);

	RECT rect = { 0, 0, w, h };
	HBRUSH limeBrush = CreateSolidBrush(RGB(124, 254, 124));
	FillRect(tempDC, &rect, limeBrush);


	HDC soureDupeDC = CreateCompatibleDC(sourceDC);
	BITMAPINFO sourcebinfo = { 0 };
	//memset(&sourcebinfo, 0, sizeof(BITMAPINFO));
	sourcebinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	sourcebinfo.bmiHeader.biHeight = h;
	sourcebinfo.bmiHeader.biWidth = w;
	sourcebinfo.bmiHeader.biSizeImage = sizeof(DWORD) * w * h;
	////binfo.bmiHeader.biSize = w * h;
	sourcebinfo.bmiHeader.biPlanes = 1;
	sourcebinfo.bmiHeader.biBitCount = sizeof(DWORD) * 8;// 32;
	sourcebinfo.bmiHeader.biCompression = BI_RGB;
	sourcebinfo.bmiHeader.biXPelsPerMeter = 0;
	sourcebinfo.bmiHeader.biYPelsPerMeter = 0;
	sourcebinfo.bmiHeader.biClrUsed = 0;
	sourcebinfo.bmiHeader.biClrImportant = 0;
	COLORREF *sbuffer = NULL;
	HBITMAP sourceBMP = CreateDIBSection(
		soureDupeDC,//[in]  HDC              hdc,
		&sourcebinfo,//[in]  const BITMAPINFO *pbmi,
		DIB_RGB_COLORS,//[in]  UINT             usage,
		(void**)&sbuffer,//[out] VOID             **ppvBits,
		NULL, //[in]  HANDLE           hSection,
		0//[in]  DWORD            offset
	);

	HBITMAP oldtempBMP2 = (HBITMAP)SelectObject(soureDupeDC, sourceBMP);
	BitBlt(soureDupeDC, 0, 0, w, h, sourceDC, 0, 0, SRCCOPY);

	double cosval = cos(degrees);
	double sinval = sin(degrees);
	
	//COLORREF colour = GetPixel(sourceDC, 1, 1);
	for (int i = 0; i < w; i++)
	{
		double hw = (w / 2.0);
		double x = (i - hw);

		for (int j = 0; j < h; j++)
		{
			double y = (j - (h / 2.0));
			//double radius = sqrt(x * x + y * y);

			double sourcex = (cosval * x + sinval * y) + hw;// cos(degrees) * x + (w / 2.0);// i;// cos(degrees) * (i - (w / 2.0)) + (w / 2.0);
			double sourcey = (-sinval * x + cosval * y) + (h / 2.0);// sin(degrees) * y + (h / 2.0);// j;// sin(degrees) * (j - (h / 2.0)) + (h / 2.0);

			int isourcex = (int)sourcex;
			int isourcey = (int)sourcey;
			//COLORREF colour = GetPixel(sourceDC, sourcex, sourcey);// (COLORREF)buffer[i * j];// GetPixel(sourceDC, sourcex, sourcey);
			//COLORREF colour =  GetPixel(sourceDC, i, j);

			//SetPixel(tempDC, i, j, colour);
			//COLORREF colour2 = GetPixel(tempDC, i, j);
			if(sourcex > 0 && sourcex < w && sourcey > 0 && sourcey < h)
			{
				if (degrees < 1260)
				{
				buffer[i * w + j] = sbuffer[isourcex * w + isourcey]; //i * w + j];// (int)(sourcex * w) + (int)sourcey];// colour;//0xFFFF00FF; //colour;// FF00FF;
				}
				else
				{
					buffer[i * w + j] = sbuffer[i * w + j];
				}
			}
			//buffer[i * j * 2] = 0xFF;
			//buffer[i * j * 3] = 0xFF;
			//buffer[i * j * 4] = 0xFF;
		}
	}
	BitBlt(destDC, 0, 0, w, h, tempDC, 0, 0, SRCCOPY);


	DeleteObject(tempBMP);
	DeleteObject(sourceBMP);

	(HBITMAP)SelectObject(soureDupeDC, oldtempBMP2);
	DeleteObject(oldtempBMP2);
	DeleteObject(soureDupeDC);

	(HBITMAP)SelectObject(tempDC, oldtempBMP);
	DeleteObject(oldtempBMP);
	DeleteObject(tempDC);
}


void DrawSeqDemo(HWND hWnd)
{
	//thank you Adrian McCarthy at https://stackoverflow.com/questions/18454380/how-to-correct-the-gdi-resource-leakage
	//for sharing this very useful way to track gdi object leakage and reminding me to release HDCs
	int gdiObjectCount = GetGuiResources(::GetCurrentProcess(), GR_GDIOBJECTS);

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: Add any drawing code that uses hdc here...
	HDC hDC_Main = GetDC(hWnd);
	HDC memoryDC = CreateCompatibleDC(hDC_Main);
	HDC tempDC = CreateCompatibleDC(memoryDC);
	HDC tempDC2 = CreateCompatibleDC(memoryDC);
	HDC tempDC3 = CreateCompatibleDC(memoryDC);


	HBITMAP memoryBMP = (HBITMAP)CreateCompatibleBitmap(hDC_Main, 200, 200);
	HBITMAP selectedBMP = (HBITMAP)SelectObject(memoryDC, memoryBMP);


	HBITMAP temp2BMP = (HBITMAP)CreateCompatibleBitmap(hDC_Main, 200, 200);
	HBITMAP oldtemp2BMP = (HBITMAP)SelectObject(tempDC2, temp2BMP);


	HBITMAP temp3BMP = (HBITMAP)CreateCompatibleBitmap(hDC_Main, 200, 200);
	HBITMAP oldtemp3BMP = (HBITMAP)SelectObject(tempDC3, temp3BMP);

	RECT rect = { 0, 0, 200, 200 };
	HBRUSH limeBrush = CreateSolidBrush(RGB(124, 254, 124));
	FillRect(tempDC2, &rect, limeBrush);
	FillRect(memoryDC, &rect, limeBrush);//WHITE_BRUSH


	HBITMAP oldtempBMP = (HBITMAP)SelectObject(tempDC, thePet->imageMask);
	BitBlt(tempDC2, 0, 0, thePet->frameWidth, thePet->frameHeight, tempDC, thePet->frameStartX, thePet->frameStartY, SRCCOPY);
	BitBlt(tempDC3, 0, 0, thePet->frameWidth, thePet->frameHeight, tempDC, thePet->frameStartX, thePet->frameStartY, NOTSRCCOPY);
	BitBlt(memoryDC, 0, 0, thePet->frameWidth, thePet->frameHeight, tempDC3, 0, 0, SRCAND);
	//DeleteObject(oldtempBMP);

	//DO NOT delete the imageMask!
	(HBITMAP)SelectObject(tempDC, thePet->imageBase);
	BitBlt(tempDC2, 0, 0, thePet->frameWidth, thePet->frameHeight, tempDC, thePet->frameStartX, thePet->frameStartY, SRCAND);

	if (testcheck == 0) {
		BitBlt(memoryDC, 0, 0, 200, 200, tempDC2, 0 * 189, 0, SRCPAINT);
		BitBlt(hDC_Main, 0, 0, 200, 200, memoryDC, 0, 0, SRCCOPY);
		testcheck += 1;
	}
	else
	{
		BitBlt(memoryDC, 0, 0, 200, 200, tempDC2, 0 * 189, 0, SRCPAINT);
		GetRotatedDC_DUMB(memoryDC, memoryDC, thePet->frameWidth, thePet->frameHeight, globalcounter * 0.314);//0.628
		BitBlt(hDC_Main, 0, 0, 200, 200, memoryDC, 0, 0, SRCCOPY);
	}

	DeleteObject(limeBrush);
	DeleteObject(temp3BMP);
	DeleteObject(temp2BMP);
	DeleteObject(memoryBMP);

	//DO NOT delete the imageMask!

	(HBITMAP)SelectObject(tempDC3, oldtemp3BMP);
	DeleteObject(oldtemp3BMP);
	DeleteObject(tempDC3);

	(HBITMAP)SelectObject(tempDC2, oldtemp2BMP);
	DeleteObject(oldtemp2BMP);
	DeleteObject(tempDC2);

	(HBITMAP)SelectObject(tempDC, oldtempBMP);
	DeleteObject(oldtempBMP);
	DeleteObject(tempDC);

	(HBITMAP)SelectObject(memoryDC, selectedBMP);
	DeleteObject(selectedBMP);
	DeleteObject(memoryDC);

	EndPaint(hWnd, &ps);
	ReleaseDC(hWnd, hdc);
	ReleaseDC(hWnd, hDC_Main);
}




void DrawClock(HWND hWnd)
{
	//thank you Adrian McCarthy at https://stackoverflow.com/questions/18454380/how-to-correct-the-gdi-resource-leakage
	//for sharing this very useful way to track gdi object leakage and reminding me to release HDCs
	int gdiObjectCount = GetGuiResources(::GetCurrentProcess(), GR_GDIOBJECTS);

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: Add any drawing code that uses hdc here...
	HDC hDC_Main = GetDC(hWnd);
	HDC memoryDC = CreateCompatibleDC(hDC_Main);
	HDC tempDC = CreateCompatibleDC(memoryDC);
	HDC tempDC2 = CreateCompatibleDC(memoryDC);
	HDC tempDC3 = CreateCompatibleDC(memoryDC);


	HBITMAP memoryBMP = (HBITMAP)CreateCompatibleBitmap(hDC_Main, 200, 200);
	HBITMAP selectedBMP = (HBITMAP)SelectObject(memoryDC, memoryBMP);


	HBITMAP temp2BMP = (HBITMAP)CreateCompatibleBitmap(hDC_Main, 200, 200);
	HBITMAP oldtemp2BMP = (HBITMAP)SelectObject(tempDC2, temp2BMP);


	HBITMAP temp3BMP = (HBITMAP)CreateCompatibleBitmap(hDC_Main, 200, 200);
	HBITMAP oldtemp3BMP = (HBITMAP)SelectObject(tempDC3, temp3BMP);

	RECT rect = { 0, 0, 200, 200 };
	HBRUSH limeBrush = CreateSolidBrush(RGB(124, 254, 124));
	FillRect(tempDC2, &rect, limeBrush);
	FillRect(memoryDC, &rect, limeBrush);//WHITE_BRUSH


	HBITMAP oldtempBMP = (HBITMAP)SelectObject(tempDC, thePet->imageMask);
	BitBlt(tempDC2, 0, 0, thePet->frameWidth, thePet->frameHeight, tempDC, thePet->frameStartX, thePet->frameStartY, SRCCOPY);
	BitBlt(tempDC3, 0, 0, thePet->frameWidth, thePet->frameHeight, tempDC, thePet->frameStartX, thePet->frameStartY, NOTSRCCOPY);
	BitBlt(memoryDC, 0, 0, thePet->frameWidth, thePet->frameHeight, tempDC3, 0, 0, SRCAND);
	//DeleteObject(oldtempBMP);

	//DO NOT delete the imageMask!
	(HBITMAP)SelectObject(tempDC, thePet->imageBase);
	BitBlt(tempDC2, 0, 0, thePet->frameWidth, thePet->frameHeight, tempDC, thePet->frameStartX, thePet->frameStartY, SRCAND);

	if (testcheck == 0) {
		BitBlt(memoryDC, 0, 0, 200, 200, tempDC2, 0 * 189, 0, SRCPAINT);
		BitBlt(hDC_Main, 0, 0, 200, 200, memoryDC, 0, 0, SRCCOPY);
		testcheck += 1;
	}
	else
	{
		BitBlt(memoryDC, 0, 0, 200, 200, tempDC2, 0 * 189, 0, SRCPAINT);
		GetRotatedDC_DUMB(memoryDC, memoryDC, thePet->frameWidth, thePet->frameHeight, globalcounter * 0.314);//0.628
		BitBlt(hDC_Main, 0, 0, 200, 200, memoryDC, 0, 0, SRCCOPY);
	}

	DeleteObject(limeBrush);
	DeleteObject(temp3BMP);
	DeleteObject(temp2BMP);
	DeleteObject(memoryBMP);

	//DO NOT delete the imageMask!

	(HBITMAP)SelectObject(tempDC3, oldtemp3BMP);
	DeleteObject(oldtemp3BMP);
	DeleteObject(tempDC3);

	(HBITMAP)SelectObject(tempDC2, oldtemp2BMP);
	DeleteObject(oldtemp2BMP);
	DeleteObject(tempDC2);

	(HBITMAP)SelectObject(tempDC, oldtempBMP);
	DeleteObject(oldtempBMP);
	DeleteObject(tempDC);

	(HBITMAP)SelectObject(memoryDC, selectedBMP);
	DeleteObject(selectedBMP);
	DeleteObject(memoryDC);

	EndPaint(hWnd, &ps);
	ReleaseDC(hWnd, hdc);
	ReleaseDC(hWnd, hDC_Main);
}




// Message handler for about box.
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    UNREFERENCED_PARAMETER(lParam);
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        return (INT_PTR)TRUE;
//
//    case WM_COMMAND:
//        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//        {
//            EndDialog(hDlg, LOWORD(wParam));
//            return (INT_PTR)TRUE;
//        }
//        break;
//    }
//    return (INT_PTR)FALSE;
//}

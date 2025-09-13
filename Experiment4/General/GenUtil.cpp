

#include "..\framework.h"
#include "GenUtil.h"
#include <memory>


void GenUtil::GetRotatedDC_DUMB(HDC destDC, HDC sourceDC, int w, int h, double degrees)
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

	double hw = (w / 2.0);
	double hh = (h / 2.0);

	//COLORREF colour = GetPixel(sourceDC, 1, 1);
	for (int i = 0; i < w; i++)
	{
		double x = (i - hw);
		int iw = i * w;

		//todo george: make faster by setting acceptable range beforehand, based on angle
		for (int j = 0; j < h; j++)
		{
			double y = (j - hh);
			//double radius = sqrt(x * x + y * y);

			int isourcex = (cosval * x + sinval * y) + hw;// cos(degrees) * x + (w / 2.0);// i;// cos(degrees) * (i - (w / 2.0)) + (w / 2.0);
			int isourcey = (-sinval * x + cosval * y) + hh;// sin(degrees) * y + (h / 2.0);// j;// sin(degrees) * (j - (h / 2.0)) + (h / 2.0);

			////int isourcex = (int)sourcex;
			////int isourcey = (int)sourcey;
			//COLORREF colour = GetPixel(sourceDC, sourcex, sourcey);// (COLORREF)buffer[i * j];// GetPixel(sourceDC, sourcex, sourcey);
			//COLORREF colour =  GetPixel(sourceDC, i, j);

			//SetPixel(tempDC, i, j, colour);
			//COLORREF colour2 = GetPixel(tempDC, i, j);
			if (isourcex > 0 && isourcex < w && isourcey > 0 && isourcey < h)
			{
				if (degrees < 1260 || true)
				{
					buffer[iw + j] = sbuffer[isourcex * w + isourcey]; //i * w + j];// (int)(sourcex * w) + (int)sourcey];// colour;//0xFFFF00FF; //colour;// FF00FF;
				}
				else
				{
					buffer[iw + j] = sbuffer[iw + j];
				}
			}
			else 
			{
				//neutral color should change depending on context...and ought to be prefilled then ignored probably
				//this whole else seems unnecessary
				//124, 254, 124
				buffer[iw + j] = 0x007CFE7C;
			}
			//buffer[i * j * 2] = 0xFF;
			//buffer[i * j * 3] = 0xFF;
			//buffer[i * j * 4] = 0xFF;
		}
	}
	BitBlt(destDC, 0, 0, w, h, tempDC, 0, 0, SRCCOPY);


	DeleteObject(limeBrush);
	DeleteObject(tempBMP);
	DeleteObject(sourceBMP);

	(HBITMAP)SelectObject(soureDupeDC, oldtempBMP2);
	DeleteObject(oldtempBMP2);
	DeleteObject(soureDupeDC);

	(HBITMAP)SelectObject(tempDC, oldtempBMP);
	DeleteObject(oldtempBMP);
	DeleteObject(tempDC);
}



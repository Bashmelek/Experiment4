
#include "..\framework.h"
#include "..\General\GenUtil.h"
#include "..\GenerPet.h"
#include "DesktopClock.h"
#include <memory>



int DesktopClock::InitPet()
{
	TCHAR pwd[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pwd);

	clockmain = (HBITMAP)LoadImage(
		NULL,
		L"ClockResources\\clockface.bmp",////\\ClockResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
	clockmask = (HBITMAP)LoadImage(
		NULL,
		L"ClockResources\\clockface_mask.bmp",////\\ClockResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);

	hourhand = (HBITMAP)LoadImage(
		NULL,
		L"ClockResources\\clocklittlehand.bmp",////\\ClockResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
	hourhandmask = (HBITMAP)LoadImage(
		NULL,
		L"ClockResources\\clocklittlehand_mask.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);


	minutehand = (HBITMAP)LoadImage(
		NULL,
		L"ClockResources\\clockbighand.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
	minutehandmask = (HBITMAP)LoadImage(
		NULL,
		L"ClockResources\\clockbighand_mask.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);


	secondhand = (HBITMAP)LoadImage(
		NULL,
		L"ClockResources\\clocksecondhand.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
	secondhandmask = (HBITMAP)LoadImage(
		NULL,
		L"ClockResources\\clocksecondhand_mask.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);

	return 0;
}


int DesktopClock::ApplyPetRender(HDC targetDC, HDC sourceDC)
{
	HDC tempDC = CreateCompatibleDC(sourceDC);
	HBITMAP tempBMP = (HBITMAP)CreateCompatibleBitmap(tempDC, 180, 180);
	HBITMAP oldtempBMP = (HBITMAP)SelectObject(tempDC, tempBMP);

	HDC memDC = CreateCompatibleDC(sourceDC);


	HBITMAP oldmemBMP = (HBITMAP)SelectObject(memDC, clockmask);
	BitBlt(targetDC, 0, 0, 180, 180, memDC, 0, 0, SRCCOPY);

	(HBITMAP)SelectObject(memDC, clockmain);
	BitBlt(targetDC, 0, 0, 180, 180, memDC, 0, 0, SRCAND);


	(HBITMAP)SelectObject(memDC, oldmemBMP);
	DeleteObject(oldmemBMP);
	DeleteObject(memDC);

	DeleteObject(tempBMP);
	(HBITMAP)SelectObject(tempDC, oldtempBMP);
	DeleteObject(oldtempBMP);
	DeleteObject(tempDC);

	return 0;
}
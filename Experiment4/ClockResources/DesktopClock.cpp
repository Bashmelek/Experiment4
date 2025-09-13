
#include "..\framework.h"
#include "..\General\GenUtil.h"
#include "..\GenerPet.h"
#include "DesktopClock.h"
#include <memory>

//_CRT_SECURE_NO_WARNINGS

//#include <chrono>
#include <ctime>


int DesktopClock::InitPet()
{
	TCHAR pwd[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pwd);

	clockmain = (HBITMAP)LoadImage(
		NULL,
		L"ClockResources\\clockfacehard.bmp",////\\ClockResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
	clockmask = (HBITMAP)LoadImage(
		NULL,
		L"ClockResources\\clockfacehard_mask.bmp",////\\ClockResources
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
	//thankyou Christopher https://stackoverflow.com/questions/35258285/how-to-use-localtime-s-with-a-pointer-in-c
	//and w3school https://www.w3schools.com/cpp/cpp_date.asp
	struct tm datetimeNow;

	time_t now = time(NULL);
	localtime_s(&datetimeNow, &now);
	int sec = datetimeNow.tm_sec;
	int imin = datetimeNow.tm_min;
	int ihour = datetimeNow.tm_hour % 12;

	double dmin = imin + (sec / 60.0);
	double dhour = ihour + (dmin / 60.0);

	const double pi = 3.14159265358979323846;

	/*HDC tempDC = CreateCompatibleDC(sourceDC);
	HBITMAP tempBMP = (HBITMAP)CreateCompatibleBitmap(tempDC, 180, 180);
	HBITMAP oldtempBMP = (HBITMAP)SelectObject(tempDC, tempBMP);*/

	HDC rotDC = CreateCompatibleDC(sourceDC);
	//thankyou Adrian Mole https://stackoverflow.com/questions/71479933/why-createcompatiblebitmap-isnt-compatible-with-createcompatibledc
	//for explaining that the default bitmap for the created dc is monochrome, so creating a bmp for it off the bat would also be monochrome
	HBITMAP rotBMP = (HBITMAP)CreateCompatibleBitmap(sourceDC, 180, 180);
	HBITMAP oldrotBMP = (HBITMAP)SelectObject(rotDC, rotBMP);

	//HDC rfullDC = CreateCompatibleDC(sourceDC);
	////bitmap must be compatible with source for reasons unknown
	//HBITMAP rfullBMP = (HBITMAP)CreateCompatibleBitmap(sourceDC, 180, 180);
	//HBITMAP oldrfullBMP = (HBITMAP)SelectObject(rfullDC, rfullBMP);


	HDC memDC = CreateCompatibleDC(sourceDC);
	HBITMAP oldmemBMP = (HBITMAP)SelectObject(memDC, clockmask);
	BitBlt(targetDC, 0, 0, 180, 180, memDC, 0, 0, SRCPAINT);
	(HBITMAP)SelectObject(memDC, clockmain);
	BitBlt(targetDC, 0, 0, 180, 180, memDC, 0, 0, SRCAND);


	/*(HBITMAP)SelectObject(memDC, minutehandmask);
	BitBlt(rfullDC, 0, 0, 180, 180, memDC, 0, 0, SRCCOPY);
	GenUtil::GetRotatedDC_DUMB(rfullDC, rfullDC, 180, 180, 0.314);
	(HBITMAP)SelectObject(memDC, minutehand);
	BitBlt(rotDC, 0, 0, 180, 180, memDC, 0, 0, SRCCOPY);
	GenUtil::GetRotatedDC_DUMB(rotDC, rotDC, 180, 180, 0.314);
	BitBlt(targetDC, 0, 0, 180, 180, rotDC, 0, 0, SRCPAINT);
*/

	(HBITMAP)SelectObject(memDC, minutehandmask);
	//BitBlt(targetDC, 0, 0, 180, 180, memDC, 0, 0, SRCPAINT);
	BitBlt(rotDC, 0, 0, 180, 180, memDC, 0, 0, SRCCOPY);
	GenUtil::GetRotatedDC_DUMB(rotDC, rotDC, 180, 180, dmin * pi / 30.0); //0.31
	BitBlt(targetDC, 0, 0, 180, 180, rotDC, 0, 0, SRCPAINT);
	(HBITMAP)SelectObject(memDC, minutehand);
	//BitBlt(targetDC, 0, 0, 180, 180, memDC, 0, 0, SRCAND);
	//BitBlt(rotDC, 0, 0, 180, 180, memDC, 0, 0, SRCCOPY);
	BitBlt(rotDC, 0, 0, 180, 180, memDC, 0, 0, SRCCOPY);
	GenUtil::GetRotatedDC_DUMB(rotDC, rotDC, 180, 180, dmin * pi / 30.0);
	BitBlt(targetDC, 0, 0, 180, 180, rotDC, 0, 0, SRCAND);

	(HBITMAP)SelectObject(memDC, hourhandmask);
	//BitBlt(targetDC, 0, 0, 180, 180, memDC, 0, 0, SRCPAINT);
	BitBlt(rotDC, 0, 0, 180, 180, memDC, 0, 0, SRCCOPY);
	GenUtil::GetRotatedDC_DUMB(rotDC, rotDC, 180, 180, dhour * pi / 6);
	BitBlt(targetDC, 0, 0, 180, 180, rotDC, 0, 0, SRCPAINT);
	(HBITMAP)SelectObject(memDC, hourhand);
	//BitBlt(targetDC, 0, 0, 180, 180, memDC, 0, 0, SRCAND);
	BitBlt(rotDC, 0, 0, 180, 180, memDC, 0, 0, SRCCOPY);
	GenUtil::GetRotatedDC_DUMB(rotDC, rotDC, 180, 180, dhour * pi / 6);
	BitBlt(targetDC, 0, 0, 180, 180, rotDC, 0, 0, SRCAND);

	(HBITMAP)SelectObject(memDC, secondhandmask);
	//BitBlt(targetDC, 0, 0, 180, 180, memDC, 0, 0, SRCPAINT);
	BitBlt(rotDC, 0, 0, 180, 180, memDC, 0, 0, SRCCOPY);
	GenUtil::GetRotatedDC_DUMB(rotDC, rotDC, 180, 180, (double)sec * pi / 30.0);
	BitBlt(targetDC, 0, 0, 180, 180, rotDC, 0, 0, SRCPAINT);
	(HBITMAP)SelectObject(memDC, secondhand);
	//BitBlt(targetDC, 0, 0, 180, 180, memDC, 0, 0, SRCAND);
	BitBlt(rotDC, 0, 0, 180, 180, memDC, 0, 0, SRCCOPY);
	GenUtil::GetRotatedDC_DUMB(rotDC, rotDC, 180, 180, (double)sec * pi / 30.0);
	BitBlt(targetDC, 0, 0, 180, 180, rotDC, 0, 0, SRCAND);


	//do not delete the object in memory, that is a loaded image
	(HBITMAP)SelectObject(memDC, oldmemBMP);
	DeleteObject(oldmemBMP);
	DeleteObject(memDC);

	//DeleteObject(rfullBMP);
	//(HBITMAP)SelectObject(rfullDC, oldrfullBMP);
	//DeleteObject(oldrfullBMP);
	//DeleteObject(rfullDC);

	DeleteObject(rotBMP);
	(HBITMAP)SelectObject(rotDC, oldrotBMP);
	DeleteObject(oldrotBMP);
	DeleteObject(rotDC);

	//DeleteObject(tempBMP);
	//(HBITMAP)SelectObject(tempDC, oldtempBMP);
	//DeleteObject(oldtempBMP);
	//DeleteObject(tempDC);

	return 0;
}
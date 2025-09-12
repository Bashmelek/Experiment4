
#include "..\framework.h"
#include "..\General\GenUtil.h"
#include "..\GenerPet.h"
#include "DesktopClock.h"
#include <memory>



int DesktopClock::InitPet()
{

	clockmain = (HBITMAP)LoadImage(
		NULL,
		L"clockface.bmp",////\\ClockResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
	clockmask = (HBITMAP)LoadImage(
		NULL,
		L"clockface_mask.bmp",////\\ClockResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);


	hourhand = (HBITMAP)LoadImage(
		NULL,
		L"clocklittlehand.bmp",////\\ClockResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
	hourhandmask = (HBITMAP)LoadImage(
		NULL,
		L"clocklittlehand_mask.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);


	minutehand = (HBITMAP)LoadImage(
		NULL,
		L"clockbighand.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
	minutehandmask = (HBITMAP)LoadImage(
		NULL,
		L"clockbighand_mask.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);


	secondhand = (HBITMAP)LoadImage(
		NULL,
		L"clocksecondhand.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
	secondhandmask = (HBITMAP)LoadImage(
		NULL,
		L"clocksecondhand_mask.bmp",////\\GameResources
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);

	return 0;
}


int DesktopClock::ApplyPetRender(HDC targetDC, HDC sourceDC)
{


	return 0;
}
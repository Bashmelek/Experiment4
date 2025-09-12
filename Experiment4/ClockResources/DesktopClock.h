#pragma once

#ifndef DKCLOCKDEF
#define DKCLOCKDEF

class DesktopClock : public GenerPet
{
private:
	HBITMAP clockmain;
	HBITMAP clockmask;
	HBITMAP hourhand;
	HBITMAP hourhandmask;
	HBITMAP minutehand;
	HBITMAP minutehandmask;
	HBITMAP secondhand;
	HBITMAP secondhandmask;

public:
	int InitPet() override;
	int ApplyPetRender(HDC targetDC, HDC sourceDC) override;
};



#endif DKCLOCKDEF

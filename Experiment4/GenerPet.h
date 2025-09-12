#pragma once



#ifndef GENERPETDEF
#define GENERPETDEF

class GenerPet
{
public:
	HBITMAP imageBase;
	HBITMAP imageMask;
	int currentFrame;
	int frameCount;
	int frameStartX;
	int frameStartY;
	int frameWidth;
	int frameHeight;

	GenerPet();
	virtual int InitPet() = 0;
	virtual int ApplyPetRender(HDC targetDC, HDC sourceDC) = 0;
	int AdvanceFrame();
};



#endif GENERPETDEF


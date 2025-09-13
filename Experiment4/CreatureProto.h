#pragma once




#ifndef CREATUREPROTODEF
#define CREATUREPROTODEF

class CreatureProto
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

		CreatureProto();
		int AdvanceFrame();
};



#endif CREATUREPROTODEF

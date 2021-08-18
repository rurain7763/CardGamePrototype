#pragma once
#include"iType.h"

class iTexture
{
public:
	iTexture();
	virtual ~iTexture();

	bool createImage(const char* szFormat, ...);
	bool createImage(int w, int h);
	void freeImage();
	
public:
	uint32 imgID;
	int width, height;
	int potWidth, potHeight;
};
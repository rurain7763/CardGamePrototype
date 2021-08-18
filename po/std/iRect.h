#pragma once
#include"iVector2D.h"

class iRect
{
public:
	iRect();
	iRect(float top, float left, float bottom, float right);
	iRect(iVector2D origin, iVector2D size);
	virtual ~iRect();

public:
	iVector2D origin;
	iVector2D size;
};

iRect iRectMake(float posX, float posY, float sizeX, float sizeY);
bool containPoint(iVector2D p, iRect rt);



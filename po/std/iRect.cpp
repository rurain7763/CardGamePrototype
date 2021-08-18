#include"iRect.h"

iRect::iRect()
{
	origin = iVector2DMakeZero;
	size = iVector2DMakeZero;
}

iRect::iRect(float top, float left, float bottom, float right)
{
	origin = iVector2DMake(left, top);
	size = iVector2DMake(right, bottom);
}

iRect::iRect(iVector2D o, iVector2D s)
{
	origin = o;
	size = s;
}

iRect::~iRect()
{
	
}

iRect iRectMake(float posX, float posY, float sizeX, float sizeY)
{
	return iRect({ posX,posY }, {sizeX,sizeY});
}

bool containPoint(iVector2D p, iRect rt)
{
	return !(p.x < rt.origin.x ||
			 p.x > rt.origin.x + rt.size.x ||
			 p.y < rt.origin.y ||
			 p.y > rt.origin.y + rt.size.y);
}


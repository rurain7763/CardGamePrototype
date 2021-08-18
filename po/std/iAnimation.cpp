#include"iAnimation.h"
#include"iStd.h"

iAnimation::iAnimation()
{
	texs = new iArray();
	frame = 0;
	curFrame = 0;
	curT = 0;
	repeat = true;
}

iAnimation::~iAnimation()
{
	delete texs;
}

void iAnimation::addTexture(iTexture* tex)
{
	texs->push_back(tex);
	frame++;
}

void iAnimation::start(float dt,iVector2D pos,iVector2D size , int anc)
{
	curT += dt;

	if (curT > _DELTATIME)
	{
		curT = 0;
		curFrame++;

		if (curFrame > frame-1)
		{
			if (repeat)
			{
				curFrame = 0;
			}
			else
			{
				curFrame = frame-1;
			}
		}
	}

	iTexture* tex = (iTexture*)texs->at(curFrame);
	drawImage(tex, pos, anc,
			  iVector2DMake(0, 0), iVector2DMake(tex->width, tex->height),
			  size,2,0);
}

void iAnimation::stop()
{

}
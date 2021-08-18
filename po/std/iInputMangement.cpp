#include"iInputMangement.h"

iInputManagement* iInputManagement::s = nullptr;

iInputManagement::iInputManagement()
{
	ms = MOUSE_LUP;
	mso = MOUSE_NONE;
	mp = iVector2DMakeZero;
	ks = KEY_NONE;
	kso = KEY_NONE;
}

iInputManagement::~iInputManagement()
{

}

void iInputManagement::update()
{
	ms &= ~MOUSE_MOVE;
	mso &= MOUSE_NONE;
	kso &= KEY_NONE;
}

void iInputManagement::mouseUpdate(unsigned int ms, iVector2D mp)
{
	mso |= ms;
	
	if (ms & MOUSE_LDOWN) this->ms &= ~MOUSE_LUP;
	if (ms & MOUSE_LUP) this->ms &= ~MOUSE_LDOWN;
	this->ms |= ms;

	this->mp = mp;
}

void iInputManagement::keyUpdate(KeySate ks, int k)
{
	if (ks == KeySateDown)
	{
		if (k == ' ')
		{
			this->ks |= KEY_SPACE;

			if (!(this->buf & KEY_SPACE))
			{
				this->kso |= KEY_SPACE;
				buf |= KEY_SPACE;
			}
		}
	}
	else if(ks == KeyStateUP)
	{
		if (k == ' ')
		{
			this->ks &= ~KEY_SPACE;
			buf &= ~KEY_SPACE;
		}
	}
}

iInputManagement* iInputManagement::share()
{
	if (!s) s = new iInputManagement();
	return s;
}


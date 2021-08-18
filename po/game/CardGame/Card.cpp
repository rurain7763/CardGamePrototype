#include"Card.h"
#include"iStd.h"

iVector2D center;

Card::Card()
{
	evn = NULL;

	handIdx = -1; //-1 : in deck 0 : in hand 
	usedCardIdx = -1;
	cardTex = NULL;
	stat = CardStatDone;
	handPos = iVector2DMakeZero;
	sp = iVector2DMakeZero; ep = iVector2DMakeZero;
	pos = iVector2DMakeZero;
	size = iVector2DMake(130, 200);
	rot = 0;
	delta = 0; _delta = .2f;
	center = iVector2DMake((DEV_WIDTH - size.x) * .5f, (DEV_HEIGHT - size.y) * .5f);
}

Card::~Card()
{
	delete cardTex;
}

void Card::move(iVector2D pos)
{
	if (stat != CardStatDone) return;
	ep = pos;
	stat = CardStatMove;
	delta = 0;
}

void Card::stop()
{
	if (stat != CardStatMove) return;
	sp = pos;
	ep = pos;
	stat = CardStatDone;
}

bool Card::useCardAnimStart(iVector2D target)
{
	if (csu != CardStatUseEnd) return false;
	csu = CardStatUseToCenter;
	del = 0;
	ep = target;
	return true;
}

void Card::paint(float dt)
{
	if (handIdx == -1) return;

	iVector2D s = size;
	float sr = 1;

	if (csu == CardStatUseToCenter)
	{
		del += dt;
		float r = del / _del;
		for (int i = 0; i < 2; i++)
			r = easeIn(0, 1, r);
		pos = linear(sp, center, r);

		if (r >= 1.f)
		{
			del = 0;
			csu = CardStatUseToDeck;
			sp = center;
		}
	}
	else if (csu == CardStatUseToDeck)
	{
		del += dt;
		float r = del / _del;
		sr = .3f;
		pos = easeIn(center, ep, r);
		pos.y -= 300 * _sin(180 * r);

		if (r >= 1.f)
		{
			if (evn) evn(this);
			del = 0;
			csu = CardStatUseEnd;
			sp = ep;
		}
	}
	else
	{
		if (stat == CardStatMove)
		{
			delta += dt;
			float r = delta / _delta;
			pos = easeIn(sp, ep, r);

			if (r >= 1.f)
			{
				if (evn) evn(this);
				stat = CardStatDone;
				delta = 0;
				sp = ep;
			}
		}
	}

	//draw
	setRGBA(1, 1, 1, 1);
	iVector2D size =
	iVector2DMake(s.x / cardTex->width * sr, s.y / cardTex->height * sr);

	drawImage(cardTex, pos, BOTTOM | RIGHT, { 0,0 },
		{ (float)cardTex->width,(float)cardTex->height },
		size,
		2, rot);
	setRGBA(1, 1, 1, 1);
}


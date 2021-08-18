#include"iFPS.h"
#include"iWindows.h"

iFPS* iFPS::s = nullptr;

iFPS::iFPS()
{
	fps = 0;
	currDt = 0;
	count = 0;
	prev = GetTickCount();
	dt = 0;
}

iFPS::~iFPS()
{

}

void iFPS::update()
{
	float curr = GetTickCount();
	dt = (curr - prev) / 1000.f;
	currDt += dt;
	count++;

	if (currDt >= 1.f)
	{
		fps = count;

		currDt = 0;
		count = 0;
	}

	timeNow = curr;
	prev = curr;
}

iFPS* iFPS::share()
{
	if (!s) s = new iFPS();
	return s;
}


#pragma once

class iFPS
{
public:
	virtual ~iFPS();

	void update();
	static iFPS* share();

private:
	iFPS();
	static iFPS* s;
public:
	unsigned long timeNow;
	int count;
	float currDt;
	float prev;
	float dt;
	int fps;
};
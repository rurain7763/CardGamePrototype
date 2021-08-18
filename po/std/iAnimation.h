#pragma once
#include"iArray.h"
#include"iTexture.h"
#include"iVector2D.h"

#define _DELTATIME 0.08f

class iAnimation
{
public:
	iAnimation();
	virtual ~iAnimation();

	void addTexture(iTexture* tex);

	void start(float dt,iVector2D pos,iVector2D size ,int anc = BOTTOM | RIGHT);
	void stop();

private:
	float curT;

public:
	iArray* texs;
	int curFrame;
	int frame;
	bool repeat;
};
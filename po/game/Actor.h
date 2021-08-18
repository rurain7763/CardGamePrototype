#pragma once
#include"iRect.h"

class iAnimation;

class Actor
{
public:
	Actor();
	virtual ~Actor();

	virtual void start();
	virtual void update(float dt);

	void draw(float dt);

public:
	const char* name;
	iAnimation* anim;
	iRect collider;
	iVector2D pos; 
	iVector2D scale;
	int order;
	bool bShow;
	bool active;
};
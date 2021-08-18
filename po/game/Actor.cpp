#include"Actor.h"
#include"iStd.h"
#include"Engine.h"

Actor::Actor()
{
	name = "";
	anim = new iAnimation();
	collider = iRect(iVector2DMakeZero, iVector2DMakeZero);
	pos = iVector2DMakeZero;
	scale = iVector2DMakeZero;
	order = 0;
	bShow = true;
	active = true;

	Engine::share()->addActor(this);
}

Actor::~Actor()
{
	delete anim;
}

void Actor::start()
{

}

void Actor::update(float dt)
{

}

void Actor::draw(float dt)
{
	if (!bShow) return;
	anim->start(dt,pos,scale);
}
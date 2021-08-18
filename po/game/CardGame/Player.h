#pragma once
#include"Character.h"

class Player : public Character
{
public:
	Player();
	virtual ~Player();

	virtual void paint(float dt);

};
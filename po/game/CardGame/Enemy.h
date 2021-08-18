#pragma once
#include"Character.h"

class Enemy : public Character
{
public:
	Enemy();
	virtual ~Enemy();

	void paint(float dt);

public:
};
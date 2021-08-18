#pragma once
#include"iVector2D.h"
#include"iString.h"
#include"iTexture.h"

class Character
{
public:
	Character();
	virtual ~Character();

	void takeDamage(Character* caster ,int damage);
	void takeWeak(int weak);
	void takeBlock(int block);

	virtual void paint(float dt) = 0;

public:
	iVector2D pos;
	iVector2D size;

	iString name;
	int cost;
	int totalCost;
	int maxHp;
	int hp;
	int weak;
	int block;
};
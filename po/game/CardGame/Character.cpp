#include "Character.h"
#include "iStd.h"

Character::Character()
{
	pos = iVector2DMakeZero;
	size = iVector2DMakeZero;

	name = "";
	cost = 0;
	totalCost = 0;
	maxHp = 0;
	hp = 0;
	weak = 0;
	block = 0;
}

Character::~Character()
{
}

void Character::takeDamage(Character* caster , int d)
{
	d *= 1.f - (caster->weak * .25f);

	if (d < 0.f) d = 0.f;
	caster->weak = 0;
	
	if (block >= d)
	{
		block -= d;
		return;
	}
	else
	{
		hp -= d - block;
		block = 0;
	}
}

void Character::takeWeak(int w)
{
	weak += w;
}

void Character::takeBlock(int b)
{
	block += b;
}

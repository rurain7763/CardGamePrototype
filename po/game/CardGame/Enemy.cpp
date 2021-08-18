#include"Enemy.h"
#include"Player.h"
#include"iStd.h"

Enemy::Enemy()
{
	maxHp = 100;
	hp = 100;
	pos = iVector2DMakeZero;
	size = iVector2DMake(70, 100);
}

Enemy::~Enemy()
{
}

void Enemy::paint(float dt)
{
	setRGBA(1, 1, 1 , 1);
	fillRect(iRect(pos - size/2, size));
	setRGBA(1, 1, 1, 1);
}

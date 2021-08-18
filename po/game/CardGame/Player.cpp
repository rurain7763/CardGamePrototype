#include"Player.h"
#include"iStd.h"

Player::Player()
{
	pos = iVector2DMake( 400, DEV_HEIGHT / 2 + 30 );
	size = iVector2DMake( 70, 100);
}

Player::~Player()
{
	
}

void Player::paint(float dt)
{
	setRGBA(0, 0, 1, 1);
	fillRect(iRect(pos - size/2 , size));
	setRGBA(1, 1, 1, 1);
}
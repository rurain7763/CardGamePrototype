#pragma once
#include"iVector2D.h"
#include"iString.h"

class iTexture;
class Character;

enum CardStat
{
	CardStatMove = 0,
	CardStatDone
};

typedef bool (*Card_anim_method)(float dt,Character* t, int num);
typedef void (*EVENT_WHEN_CARDSTATDONE)(void* parm);
class Card
{
public:
	Card();
	virtual ~Card();

	void stop();
	void move(iVector2D pos);
	bool useCardAnimStart(iVector2D target);
	virtual void paint(float dt);

private:
	enum CardStatUse
	{
		CardStatUseToCenter = 0,
		CardStatUseToDeck,
		CardStatUseEnd
	};

	float del = 0, _del = .5f;
	CardStatUse csu = CardStatUseEnd;

public:
	//Card Attribute
	iString name;
	iString	tear;
	iString effect;
	int damage;
	int	cost;
	iString	text;
	//_Card Attribute

	EVENT_WHEN_CARDSTATDONE evn;
	Card_anim_method cardAnim;

	int handIdx;
	int usedCardIdx;

	iTexture* cardTex;
	CardStat stat;
	iVector2D handPos;

	iVector2D pos;
	iVector2D sp;
	iVector2D ep;
	iVector2D size;
	float rot;

	float delta, _delta;
};


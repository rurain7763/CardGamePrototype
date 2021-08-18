#pragma once
#include"iVector2D.h"
#include"Card.h"

#define DRAW_CARD_NUM 6 

//Test==========================================================>
struct CardInfo;
struct _Card;
//Test==========================================================>


class Character;
class iTexture;
class CardAnimationManager;

class Deck
{
public:
	Deck();
	virtual ~Deck();

	void createCard();
	void useCard(Card* card,Character* target,int targetNum);
	void discards(int* idx,int num);
	void discard(int idx = -1);
	void draw();
	void draw(int num,const char* cardName = "None");

	bool isAllCardAnimDone();
	void paint(float dt);

private:
	enum DrawStat
	{
		DrawStatToDeck = 0,
		DrawStatPickCard,
		DrawStatToHand,

		DrawStatNone
	};

	DrawStat dStat;

private:
	CardAnimationManager* cam;

public:
	Card* allCard;
	int allCardNum;
	
	Card** hand;
	int handNum;
	
	Card** usedCard;
	int usedCardNum;

	iVector2D pos;
};

struct CardAnimationFormmat
{
	Card_anim_method cardAnim;
	Character* cha;
	int num;
};

class CardAnimationManager
{
public:
	CardAnimationManager();
	virtual ~CardAnimationManager();

	void update(float dt);
	bool animStart(CardAnimationFormmat caf);

public:
	CardAnimationFormmat cardAnim[10];
	int num;
};

void initSkillMapBySilent();
bool silentStrike(float dt, Character* t, int num);
bool silentNeutralize(float dt, Character* t, int num);
bool silentDefend(float dt, Character* t, int num);
bool silentSurvivor(float dt, Character* t, int num);
bool silentAcrobatics(float dt, Character* t, int num);
bool silentBackflip(float dt, Character* t, int num);
bool silentBane(float dt, Character* t, int num);
bool silentBladeDance(float dt, Character* t, int num);




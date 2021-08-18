#pragma once
#include"GameAttribute.h"
#include"iVector2D.h"

class iTextReader;
class Deck;
class Player;
class Stage;
class Card;

struct CheckMessage
{
public:
	int reason; //-1 : continue ,0 : player dead , 1: clear
	bool bContinue;
};

extern Card* sel;
class GameManager
{
public:
	GameManager();
	virtual ~GameManager();

	void start();
	void checkGameContinue();
	void update(float dt);

private:
	void loadStage();
	void freeStagePop();

	void loadGamePop();
	void updateGamePop(float dt);
	void freeGamePop();

public:
	iTextReader* texReader;

	Deck* deck;
	Player* p;

	Stage* stage; //enemy in
	Stage* curStage;

	CheckMessage cm;

	bool playerTurn;
	int turn;
};

void addNumberTex(iVector2D spawnP, int num);
void paintNumTex(float dt);

void addStateTex(iVector2D start , iVector2D end , const char* szFormat ,...);
void paintStateTex(float dt);

void addWeakTex(iVector2D upPos, iVector2D downPos, int* weak);
void paintWeakTex(float dt);

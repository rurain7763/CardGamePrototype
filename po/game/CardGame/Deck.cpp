#include"Deck.h"
#include"GameManager.h"
#include"iStd.h"
#include"Proc.h"
#include"Player.h"

static map<iString, Card_anim_method>* skillMap;
static iBitmap** cardFrame;
int frameImageNum = 3;

Deck::Deck()
{
	skillMap = new map<iString, Card_anim_method>();
	cam = new CardAnimationManager();

	allCard = NULL;
	allCardNum = 0;

	pos = iVector2DMake(100 , DEV_HEIGHT - 150);

	iGraphics* ig = iGraphics::share();
	cardFrame = ig->sliceBitmap(3,1,"assets/Proc/CardFrame.png");

	hand = new Card * [DRAW_CARD_NUM];
	handNum = 0;

	usedCard = new Card * [DRAW_CARD_NUM];
	usedCardNum = 0;

	dStat = DrawStatNone;
	//pos.y = dev_height + cardheight/2
}

Deck::~Deck()
{
	delete skillMap;
	delete cam;
	delete[] allCard;
	delete[] hand;
	delete[] usedCard;

	for (int i = 0; i < frameImageNum; i++)
		delete cardFrame[i];
	delete cardFrame;
}

void useCardEvent(void* parm)
{
	Card* c = (Card*)parm;

	c->handIdx = -1;
	c->evn = NULL;
	
	//TODO : make player fuct useCost and gainCost
	gm->p->cost -= c->cost;
}

#define st (*dm)
SubTitle* dm;
void Deck::createCard()
{
	//TODO : must set player type -------------------------
	const char* playerType = "Silent";
	dm = gm->texReader->subt;
	MainTitle* mt = &st["CardSetting"][gm->p->name];
	
	iString cn = "CardSetting//CardNum";
	int start = -1, end = -1;
	cn.find("CardSetting//", start, end);
	cn.insert(end, playerType);

	allCardNum = charToint((*gm->texReader)[cn].str);
	allCard = new Card[allCardNum];

	MainTitle::iterator itr = mt->begin();
	if (!strcmp(playerType,"Silent")) initSkillMapBySilent();
	//-----------------------------------------------------

	for (int i=0 ; itr != mt->end(); itr++,i++)
	{
		Card* card = &allCard[i];

		map<iString, iString>& m = itr->second;

		card->sp = pos;

		//read attribute
		card->name = m["Name"];
		card->tear = m["Tear"];
		card->cost = charToint(m["Cost"].str);
		card->effect = m["Effect"];
		card->cardAnim = (*skillMap)[card->name];
		
		//drawcard
		iString type = m["Type"];
		iBitmap* bmp = NULL;
		if (type == "Attack") bmp = cardFrame[2];
		else if (type == "Skill") bmp = cardFrame[1];
		else if (type == "Power") bmp = cardFrame[0];
		
		iGraphics* ig = iGraphics::share();
		int w = bmp->width; int h = bmp->height;
		ig->init(w,h);
		ig->setColor(1, 1, 1, 1);
		ig->fillRect(iRect({ 120, 100 }, { (float)w * .8f, (float)h * .5f }));
		ig->setColor(1, 1, 1, 1);
		ig->drawImage(	bmp, { 0,0 }, 0,
						{ 0,0 }, { (float)w,(float)h },
						{ 1,1 },
						2, 0, 0);

		ig->fontSize = 150;
		ig->setFontName("Arial");
		ig->setStringColor(1, 0, 1, 1);
		iVector2D costP = iVector2DMake(120, 100);
		ig->drawString(costP , 0 , "%d" ,card->cost);

		ig->fontSize = 100;
		iVector2D nameP = iVector2DMake(w * .5f, h * .12f);
		ig->setStringColor(1, 1, 1, 1);
		ig->drawString(nameP, HCENTER | VCENTER, "%s", card->name.str);

		ig->fontSize = 100;
		iVector2D textP = iVector2DMake(w*.25f, h *.65f);
		ig->setStringColor(1, 1, 1, 1);
		iBitmap* text = ig->getStringBmpInRect( w - 510 , 400 , 30, card->effect.str);
		ig->drawImage(	text, textP, BOTTOM | RIGHT, { 0,0 },
						{ (float)text->width,(float)text->height }, { 1,1 }, 2, 0 ,
						REVERSE_NONE);
		delete text;
		card->cardTex = ig->getTexture();
	}
}

void Deck::useCard(Card* c, Character* t, int num)
{
	if (gm->p->cost < c->cost) return;
	CardAnimationFormmat caf{c->cardAnim,t,num};
	if (cam->animStart(caf))
	{
		Log("order success\n");
	}
	else
	{
		Log("order failed\n");
		return;
	}

	c->handPos = pos;
	c->evn = useCardEvent;
	c->stat = CardStatDone;
	c->useCardAnimStart(pos);

	for (int i = c->handIdx; i < handNum - 1; i++)
	{
		hand[i] = hand[i + 1];
		hand[i]->handIdx = i;
	}
	handNum--;

	usedCard[usedCardNum] = c;
	c->usedCardIdx = usedCardNum;
	usedCardNum++;

	iVector2D offP = iVector2DMakeZero;
	for (int i = 0; i < handNum; i++)
	{
		offP.x = 
		DEV_WIDTH * .5f - (hand[i]->size.x * handNum) * .5f + 100 * i;
		offP.y = hand[i]->handPos.y;

		hand[i]->stat = CardStatDone;
		hand[i]->handPos = offP;
		hand[i]->move(offP);
	}
}

void Deck::discards(int* idx, int num)
{
	if (num == 0)
	{
		//discards random 
	}
	else
	{

	}
}

void Deck::discard(int idx)
{
	if (handNum == 0) return;

	if (idx == -1)
	{
		//random
		int rand = random() % handNum;

		int n = handNum - 1;
		for (int i = rand; i < n; i++)
		{
			hand[i] = hand[i + 1];
		}
		handNum--;

		iVector2D offP;
		for (int i = 0; i < handNum; i++)
		{
			offP.x =
			DEV_WIDTH * .5f -
			(hand[i]->size.x * handNum) * .5f +
			100 * i;
			offP.y = pos.y;

			hand[i]->stop();
			hand[i]->handPos = offP;
			hand[i]->move(offP);
		}
	}
	else
	{
		//discard target card
	}
}

void Deck::draw()
{
	if (dStat != DrawStatNone) return;

	for (int i = 0; i < handNum; i++)
	{
		hand[i]->stat = CardStatDone;
		hand[i]->move(iVector2DMake(pos.x, pos.y));
	}

	usedCardNum = 0;
	handNum = 0;
	dStat = DrawStatToDeck;
}

void Deck::draw(int num, const char* cardName)
{
	if (!strcmp("None", cardName))
	{
		//random
		while (1)
		{
			int rand = random() % allCardNum;
			Card* card = &allCard[rand];

			bool ok = false;
			for (int i = 0; i < handNum; i++)
			{
				if (hand[i] == card)
				{
					ok = false;
					break;
				}
			}

			if (ok)
			{
				hand[handNum] = card;
				handNum++;
				break;
			}
		}

		iVector2D offP;
		for (int i = 0; i < handNum; i++)
		{
			offP.x =
			DEV_WIDTH * .5f -
			(hand[i]->size.x * handNum) * .5f +
			100 * i;
			offP.y = pos.y;

			hand[i]->stop();
			hand[i]->handPos = offP;
			hand[i]->move(offP);
		}
	}
	else
	{

	}
}

bool Deck::isAllCardAnimDone()
{
	return !cam->num;
}

void Deck::paint(float dt)
{
	cam->update(dt);

	if (dStat == DrawStatToDeck)
	{
		bool allCardDone = true;
		for (int i = 0; i < handNum; i++)
			if (hand[i]->stat == CardStatMove) allCardDone = false;

		if (allCardDone) dStat = DrawStatPickCard;	
	}
	else if (dStat == DrawStatPickCard)
	{
		while (handNum < DRAW_CARD_NUM)
		{
			int rnd = random() % allCardNum;
			Card* c = &allCard[rnd];

			bool exist = false;
			for (int i = 0; i < handNum; i++)
			{
				if (hand[i] == c)
				{
					exist = true;
					break;
				}
			}

			if (exist) continue;
			c->handIdx = handNum;
			hand[handNum] = c;
			handNum++;
		}

		iVector2D offP;
		for (int i = 0; i < handNum; i++)
		{		
			offP.x = 
			DEV_WIDTH * .5f -
			(hand[i]->size.x * handNum) * .5f +
			100 * i;
			offP.y = pos.y;

			hand[i]->stat = CardStatDone;
			hand[i]->handPos = offP;
			hand[i]->move(offP);			
		}

		dStat = DrawStatToHand;
	}
	else if (dStat == DrawStatToHand)
	{
		bool allCardDone = true;
		for (int i = 0; i < handNum; i++)
			if (hand[i]->stat == CardStatMove) allCardDone = false;

		if (allCardDone)
		{
			dStat = DrawStatNone;
		}
	}

	int h = handNum - 1;
	float offR = -h*5;
	for (int i = h; i >-1; i--)
	{
		if (hand[i] == sel) continue;
	
		if (h == 0) hand[i]->rot = 0;
		else hand[i]->rot = offR + (-offR*2 / h) * (h-i);
		hand[i]->paint(dt);
	}
	
	if (sel)
	{
		sel->rot = 0;
		sel->paint(dt);
	}
	
	for (int i = 0; i < usedCardNum; i++)
	{
		usedCard[i]->paint(dt);
	}

	setRGBA(1, 1, 1, 1);
	iVector2D size = iVector2DMake(130, 200);
	fillRect(iRect(pos, size));
	setRGBA(1, 1, 1, 1);
}

//----------------------------------------CardAnimationManager
float cardAnimDelta;
CardAnimationManager::CardAnimationManager()
{
	cardAnimDelta = 0.f;
	memset(cardAnim, 0, sizeof(CardAnimationFormmat) * 10);
	num = 0;
}

CardAnimationManager::~CardAnimationManager()
{

}

void CardAnimationManager::update(float dt)
{
	if (num == 0) return;

	CardAnimationFormmat* caf = &cardAnim[0];
	
	if (caf->cardAnim(dt, caf->cha, caf->num))
	{
		num--;
		for (int i = 0; i < num; i++)
			cardAnim[i] = cardAnim[i + 1];
		cardAnimDelta = 0;	
	}
}

bool CardAnimationManager::animStart(CardAnimationFormmat caf)
{
	if (num >= 9)
	{
		return false;
	}

	cardAnim[num] = caf;
	num++;
	return true;
}

//----------------------------CardAnim
//Silent
void initSkillMapBySilent()
{
	(*skillMap)["Strike"] = silentStrike;
	(*skillMap)["Neutralize"] = silentNeutralize;
	(*skillMap)["Defend"] = silentDefend;
	(*skillMap)["Survivor"] = silentSurvivor;
	(*skillMap)["Acrobatics"] = silentAcrobatics;
	(*skillMap)["Backflip"] = silentBackflip;
	(*skillMap)["Bane"] = silentBane;
	(*skillMap)["BladeDance"] = silentBladeDance;
}

bool silentStrike(float dt, Character* t, int num)
{
	cardAnimDelta += dt;
	float r = cardAnimDelta / 3.f;

	if (cardAnimDelta < 1.f)
	{
		Log("Strike Animation...\n");
	}
	else if (cardAnimDelta >= 1.f)
	{
		int d = atoi(st["CardSetting"]["Silent"]["Strike"]["Damage"].str);

		for (int i = 0; i < num; i++)
		{
			t[i].takeDamage(gm->p,d);
			addNumberTex(t[i].pos, d);
		}
		return true;
	}

	return false;
}

bool silentNeutralize(float dt, Character* t, int num)
{
	cardAnimDelta += dt;
	float r = cardAnimDelta / 3.f;

	if (cardAnimDelta < 1.f)
	{
		Log("Neutralize Animation...\n");
	}
	else if (cardAnimDelta >= 1.f)
	{
		int d = atoi(st["CardSetting"]["Silent"]["Neutralize"]["Damage"].str);
		int w = atoi(st["CardSetting"]["Silent"]["Neutralize"]["Weak"].str);

		for (int i = 0; i < num; i++)
		{
			iVector2D pos = t[i].pos;
			t[i].takeDamage(gm->p,d);
			addNumberTex(pos, d);

			iVector2D end = pos;
			end.y -= 100.f;
			t[i].takeWeak(2);
			addStateTex(pos, end, "+%d Weak",w);
		}

		return true;
	}

	return false;
}

bool silentDefend(float dt, Character* t, int num)
{
	cardAnimDelta += dt;
	float r = cardAnimDelta / 3.f;

	if (cardAnimDelta < 1.f)
	{
		Log("Defend Animation...\n");
	}
	else if (cardAnimDelta >= 1.f)
	{
		int b = atoi(st["CardSetting"]["Silent"]["Defend"]["Block"].str);
		gm->p->takeBlock(b);

		iVector2D end = gm->p->pos;
		end.y -= 100.f;
		addStateTex(gm->p->pos, end, "+%d Block",b);

		return true;
	}

	return false;
}

bool silentSurvivor(float dt, Character* t, int num)
{
	//if(cardAnimDelta == 0.f) gm->deck->discard();

	cardAnimDelta += dt;
	float r = cardAnimDelta / 3.f;

	if (cardAnimDelta < 1.f)
	{
		Log("Survivor Animation...\n");
	}
	else if (cardAnimDelta >= 1.f)
	{
		int b = atoi(st["CardSetting"]["Silent"]["Survivor"]["Block"].str);
		gm->p->takeBlock(b);
		iVector2D end = gm->p->pos;
		end.y -= 100.f;
		addStateTex(gm->p->pos, end, "+%d Block",b);

		return true;
	}

	return false;
}

bool silentAcrobatics(float dt, Character* t, int num)
{
	return true;

	if (cardAnimDelta == 0.f)
	{
		int d = atoi(st["CardSetting"]["Silent"]["Acrobatics"]["Draw"].str);	
		gm->deck->draw(d);
	}

	cardAnimDelta += dt;
	float r = cardAnimDelta / 3.f;

	if (cardAnimDelta < 1.f)
	{
		Log("Acrobatics Animation...\n");
	}
	else if (cardAnimDelta >= 1.f)
	{
		return true;
	}

	return false;
}

bool silentBackflip(float dt, Character* t, int num)
{
	return true;
}

bool silentBane(float dt, Character* t, int num)
{
	return true;
}

bool silentBladeDance(float dt, Character* t, int num)
{
	return true;
}






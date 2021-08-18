#include"CardGame/GameManager.h"
#include"CardGame/Player.h"
#include"CardGame/Enemy.h"
#include"CardGame/Deck.h"
#include"Stage.h"
#include"Loading.h"
#include"Menu.h"
#include"Proc.h"
#include"iStd.h"

static iPopup* gamePop;
static iPopup* stagePop;
static iInputManagement* ip;

GameManager::GameManager()
{
	ip = iInputManagement::share();
	texReader = new iTextReader();
	stage = new Stage[STAGE_NUMX * STAGE_NUMY];

	deck = new Deck();
	playerTurn = true;
	turn = 0;
}

GameManager::~GameManager()
{
	delete texReader;
	delete p;
	delete stage;
	delete deck;

	freeStagePop();
	freeGamePop();
}

void GameManager::checkGameContinue()
{
	bool clear = true;
	for (int i = 0; i < curStage->enemyNum; i++)
	{
		if (curStage->enemy[i].hp > 0)
		{
			clear = false;
			break;
		}
	}
	
	if(clear)	
	{
		cm.bContinue = false;
		cm.reason = 1;
		stagePop->show(true);
		p->cost = p->totalCost;
		return;
	}
	else if (p->hp <= 0)
	{
		//player dead
		cm.bContinue = false;
		cm.reason = 0;
		return;
	}

	cm.bContinue = true;
	cm.reason = -1;
}

#define dm (*texReader)
void GameManager::start()
{	
	//1. load data
	int fileLen = -1;
	iString data = readFile(fileLen, "assets/DataFile.txt");
	texReader->read(data);

	//2. setting player
	iString att[3] =
	{
		"CharacterSetting//Attribute/Name",
		"CharacterSetting//Attribute/Hp",
		"CharacterSetting//Attribute/TotalCost"
	};
	
#if 1
	playerName = "Silent";
#endif

	for (int i = 0; i < 3; i++)
	{
		int start, end = -1;
		att[i].find("CharacterSetting//", start, end);
		att[i].insert(end, playerName);
	}

	p = new Player();
	p->name = dm[att[0]];
	p->maxHp = charToint(dm[att[1]].str);
	p->hp = p->maxHp;
	p->totalCost = charToint(dm[att[2]].str);
	p->cost = p->totalCost;
	
	//3. make stage
	loadStage();
	curStage = NULL;
	cm.bContinue = false;
	cm.reason = 1;

	loadGamePop();
	gamePop->show(true);

	deck->createCard();
	deck->draw();
	
	stagePop->show(true);
}

void controllCard()
{
	Deck* deck = gm->deck;

	if (!sel)
	{
		for (int i = 0; i < deck->handNum; i++)
		{
			Card* c = deck->hand[i];

			iRect rt = iRect(c->ep, c->size);
			if (containPoint(ip->mp, rt) &&
				ip->ms & MOUSE_LDOWN)
			{
				sel = c;
				break;
			}
			else if (containPoint(ip->mp, rt))
			{
				iVector2D up = c->handPos;
				up.y -= 70.f;
				c->stop();
				c->move(up);
			}
			else
			{
				c->move(c->handPos);
			}
		}
	}
	else
	{
		iRect rt = iRect({ 0,0 }, { DEV_WIDTH,DEV_HEIGHT / 2 + 150 });
		setRGBA(0, 0, 0, .5f);
		fillRect(rt);
		setRGBA(1, 1, 1, 1);

		if (ip->ms & MOUSE_LDOWN)
		{
			sel->stop();
			sel->move(ip->mp - sel->size / 2);
		}
		else
		{
			if (containPoint(ip->mp, rt))
				deck->useCard(sel, gm->curStage->enemy, gm->curStage->enemyNum);
			sel = NULL;
		}
	}
}

float test = 0;
Card* sel = NULL;
void GameManager::update(float dt)
{
#if 1
	if (cm.bContinue)
	{
		checkGameContinue();

		if (curStage->type != StageTypeEvent ||
			curStage->type != StageTypeShop)
		{
			if (playerTurn)
			{				
				controllCard();
			}
			else
			{
				//enemy turn
				test += dt;

				if (test >= 5.f)
				{
					//TODO gain cost anim
					p->takeDamage(&curStage->enemy[0], 50);
					p->cost = p->totalCost;
					p->block = 0;
					deck->draw();
					playerTurn = true;
					test = 0;
				}
			}

			updateGamePop(dt);

			//-----------------------------------paint
			p->paint(dt);

			//enemy paint
			int num = curStage->enemyNum;

			for (int i = 0; i < num; i++)				
				curStage->enemy[i].paint(dt);
			
			deck->paint(dt);
			gamePop->paint(dt);

			//num tex
			paintWeakTex(dt);
			paintStateTex(dt);
			paintNumTex(dt);
		}
		else // event and shop
		{

		}
	}
	else
	{
		switch (cm.reason)
		{
		case 0: // player dead
			setLoading(SCENE_MENU, freeProc, loadMenu);
			break;
		case 1: // enemy clear
			stagePop->paint(dt);
			break;
		}
	}
#endif
}

//-----------------------------------Stage
static Botton** btn;
static int toStage = -1;
void onClick(void* parm)
{
	Botton* b = (Botton*)parm;
	toStage = b->idx;
	stagePop->show(false);
}

void onMouseOverlap(void* parm)
{
	Botton* b = (Botton*)parm;
	b->setColor(1, 0, 0, 1);

	Stage* s = &gm->stage[b->idx];
	for (int i = 0; i < s->idx; i++)
	{
		int idx = s->next[i];

		b = btn[idx];
		b->setColor(1, 0, 0, 1);
	}
}

void popEvent(iPopupStat stat)
{
	if (stat == iPopupStatClose)
	{
		gm->curStage = &gm->stage[toStage];
		gm->cm.bContinue = true;
		gm->cm.reason = -1;
		gm->turn = 0;
		toStage = -1;
		iVector2D pP = gm->p->pos;
		addWeakTex(pP, iVector2DMake(pP.x, pP.y - 100), &gm->p->weak);

		for (int i = 0; i < gm->curStage->enemyNum; i++)
		{
			Enemy* e = &gm->curStage->enemy[i];

			addWeakTex(e->pos, iVector2DMake(e->pos.x, e->pos.y - 100), &e->weak);
		}
	}
}

void GameManager::loadStage()
{
	int sn = STAGE_NUMX * STAGE_NUMY;
	stage = new Stage[sn];

	int i, j ,rand;
	Stage* s = NULL;
	for (j = 0; j < STAGE_NUMY; j++)
	{
		for (i = 0; i < STAGE_NUMX; i++)
		{
			rand = 1 + random() % 3;
			s = &stage[STAGE_NUMX * j + i];
			s->sn = STAGE_NUMX * j + i;
			s->type = StageType(random() % StageTypeMax);
			s->enemyNum = rand;
			s->enemy = new Enemy[rand];

			iVector2D off = iVector2DMake(680, DEV_HEIGHT / 2 + 30);
			for (int i = 0; i < rand; i++)
			{
				//TODO : enemy setting
				s->enemy[i].hp = 100;
				s->enemy[i].pos = iVector2DMake(off.x + 150 * i, off.y);
				s->enemy[i].size = iVector2DMake(70, 100);
			}
		}
	}

	while (1)
	{
		for (i = 0; i < STAGE_NUMX; i++)
		{
			stage[i].connected = false;
		}

		int active = 0;

		for (i = 0; i < STAGE_NUMX; i++)
		{
			if (random() % 4 / 3) continue;
			active++;
			stage[i].connected = true;
		}

		if (active > STAGE_NUMX/2) break;
	}

	for (j = 0; j < STAGE_NUMY-1; j++)
	{
		for (i = 0; i < STAGE_NUMX; i++)
		{
			int s = STAGE_NUMX * j + i;

			if (stage[s].connected)
			{
				while (1)
				{
					stage[s].idx = 0;

					for (int k = 0; k < STAGE_NUMX; k++)
					{
						if (stage[s].idx > 2) break;
						if (random() % 3 / 2) continue;
						int t = STAGE_NUMX * (j + 1) + k;
						stage[t].connected = true;
						stage[s].next[stage[s].idx] = stage[t].sn;
						stage[s].idx++;
					}

					if (stage[s].idx > 0) break;
				}
			}
		}
	}

#if 0
	boss[0].connected = true;
	boss[0].enemyNum = 1;
	boss[0].enemy = new Enemy[1];
	boss[0].sn = 101;
	boss[0].type = StageTypeBoss;

	for (i = 0; i < STAGE_NUMX; i++)
	{
		int s = STAGE_NUMX * (STAGE_NUMY - 1) + i;

		if (stage[s].connected)
		{
			stage[s].next[stage[s].idx] = boss[0].sn;
			stage[s].idx++;
		}
	}
#endif

//---------------------------------------------------------------Make StagePopup
	stagePop = new iPopup();
	stagePop->style = iPopupStyleMove;
	stagePop->sp = iVector2DMake(-DEV_WIDTH, DEV_HEIGHT / 2);
	stagePop->ep = iVector2DMake(DEV_WIDTH / 2, DEV_HEIGHT / 2);
	stagePop->_delta = .5f;
	stagePop->event = popEvent;

	int rw = 30; int rh = 30;
	int inX = 30; int inY = 40;

	btn = new Botton * [STAGE_NUMX * STAGE_NUMY];
	for (int j = 0; j < STAGE_NUMY; j++)
	{
		int count = 0;

		for (int i = 0; i < STAGE_NUMX; i++)
		{
			if (stage[STAGE_NUMX * j + i].connected)
			{
				count++;
			}
		}

		int width = rw * count + inX * (count - 1);
		int offX = DEV_WIDTH / 2 - width / 2;
		int offY = DEV_HEIGHT / 2 - ((STAGE_NUMY * rh) + ((STAGE_NUMY - 1) * inY)) / 2;
		int stack = 0;

		Botton* b = new Botton[count];
		for (int i = 0; i < STAGE_NUMX; i++)
		{
			if (!stage[STAGE_NUMX * j + i].connected) continue;

			Botton* t = &b[stack];

			t->relatePos = iVector2DMake(offX + (inX + rw) * stack, offY + (inY + rh) * j);
			t->relateSize = { (float)rw,(float)rh };
			t->idx = stage[STAGE_NUMX * j + i].sn;
			if (b->idx/STAGE_NUMX == 0) t->initColor(1, 1, 1, 1);
			else t->initColor(.5, .5, .5, 1);
			t->onClick = onClick;
			t->onMouseOverlap = onMouseOverlap;

			stagePop->addObject(t);
			btn[t->idx] = t;
			stack++;
		}
	}
}

void GameManager::freeStagePop()
{
	delete stagePop;

	for (int i = 0; i < STAGE_NUMX * STAGE_NUMY; i++)
		if (btn[i]) delete btn[i];
	delete btn;
}

//---------------------------------------------Game Popup
Botton* endTurnBtn;
iPopupFillBar* fbar;
iPopupFillBar* enemyFbar;
iPopupImage* topBar;
iPopupTextBox* costTextBox;
iPopupTextBox* blockTextBox;

void onClickEndTurnBtn(void* parm)
{
	if (!gm->playerTurn || !gm->deck->isAllCardAnimDone()) return;

	Deck* d = gm->deck;

	for (int i = 0; i < d->handNum; i++)
	{
		d->hand[i]->stat = CardStatDone;
		d->hand[i]->move(d->pos);
	}

	gm->playerTurn = false;
}

void onMouseOverlapEndTurnBtn(void* parm)
{
	Botton* b = (Botton*)parm;
	
	if (b->idx == 0) //draw botton
	{
		b->setColor(1, 1, 1, 1);
	}
}

struct NumberTex
{
	iTexture* tex;
	iVector2D pos;
	bool active;
	int degree;
	iVector2D force;
	float delta , _delta;
	bool tick;

	void init()
	{
		tex = NULL;
		pos = iVector2DMakeZero;
		active = false;
		degree = 0.f;
		force.x = 0.f;
		force.y = 400.f;
		delta = 0.f;
		_delta = .1f;
		tick = true;
	}

	bool paint(float dt)
	{
		delta += dt;
		float scale = 1.f;
		float alpha = 1.f;
		float rate = delta / _delta;
		float r,g,b;

		if (tick)
		{
			scale = easeIn(0.f, 1.f, rate);
			r = scale;
			g = 0.f;
			b = 0.f;
			if (r >= 1.f)
			{
				delta = 0.f;
				_delta = 2.f;
				tick = false;
			}
		}
		else
		{
			r = 1.f; g = 1.f; b = 1.f;
			alpha = linear(1.f, 0.f, rate);
			scale = linear(1.f, 0.f, rate);
		}

		degree += dt * 300.f;
		float c = _cos(degree);
		pos.x += force.x * dt;

		if (degree >= 180.f)
		{
			force.y += 1000.f * dt;
			pos.y += force.y * dt;
		}
		else
		{
			pos.y -= c * 400.f * dt;
		}

		if (force.x > 0)
		{
			force.x -= 100.f * dt;
			if (force.x <= 0) force.x = 0;
		}
		else if(force.x < 0)
		{
			force.x += 100.f * dt;
			if (force.x >= 0) force.x = 0;
		}

		setRGBA(r, g, b, alpha);
		drawImage(tex, pos, BOTTOM | RIGHT,
				  { 0,0 },
				  { (float)tex->width,(float)tex->height },
				  { scale,scale },
				  2, 0);
		setRGBA(1, 1, 1, 1);

		return pos.y > DEV_HEIGHT;
	}
};

struct StateTex
{
	bool active;
	iTexture* tex;
	iVector2D start;
	iVector2D pos;
	iVector2D end;
	float delta, _delta;

	void init()
	{
		active = false;
		tex = NULL;
		start = iVector2DMakeZero;
		end = iVector2DMakeZero;
		pos = iVector2DMakeZero;
		delta = 0.f;
		_delta = 1.f;
	}

	bool paint(float dt)
	{
		delta += dt;
		float rate = delta / _delta;

		float r, g, b, alpha;
		float scale = .5f;

		r = rate; g = 1-rate; b = 1-rate;
		alpha = 1.f;
		pos = easeIn(start, end, rate);

		setRGBA(r, g, b, alpha);
		drawImage(tex, pos, VCENTER | HCENTER,
			{ 0,0 },
			{ (float)tex->width,(float)tex->height },
			{ scale,scale },
			2, 0);
		setRGBA(1, 1, 1, 1);

		return rate >= 1.f;
	}
};

struct WeakTex
{
	bool active;
	iTexture* tex;
	iVector2D upPos;
	iVector2D downPos;
	iVector2D pos;
	bool tick;
	float delta, _delta;
	int* weak;
	int curWeak;

	void init()
	{
		active = false;
		tex = NULL;
		upPos = iVector2DMakeZero;
		downPos = iVector2DMakeZero;
		pos = iVector2DMakeZero;
		delta = 0.f;
		_delta = 1.f;
		tick = false;
		weak = NULL;
		curWeak = 0;
	}

	void paint(float dt)
	{
		if (*weak == 0) return;

		if (curWeak != *weak)
		{
			iGraphics* g = iGraphics::share();
			iVector2D size = iVector2DMake(50, 50);
			g->init(size.x, size.y);
			g->setColor(.8f, .2f, .2f, 1.f);
			g->fillRect(iRect({ 0,0 }, size));
			g->setColor(1, 1, 1, 1);
			g->fontSize = 23.f;
			g->setStringColor(1, 1, 1, 1);
			iVector2D pos = size * .5f;
			g->drawString(pos, VCENTER | HCENTER, "%d", *weak);
			g->setStringColor(1, 1, 1, 1);
			tex = g->getTexture();
			curWeak = *weak;
		}

		delta += dt;
		float _r = delta / _delta;
		if (_r >= 2.f) delta = 0.f;
		
		tick = (int)_r;

		if (!tick)
		{
			//up
			pos = linear(downPos, upPos, _r);
		}
		else
		{
			//down
			pos = linear(upPos, downPos, _r - 1.f);
		}

		setRGBA(1, 1, 1, 1);
		drawImage(tex, pos, VCENTER | HCENTER,
				  { 0,0 },
				  { (float)tex->width,(float)tex->height },
				  { 1,1 },
				  2, 0);
		setRGBA(1, 1, 1, 1);
	}
};

#define NUM_TEX 10
NumberTex* _numTex;
NumberTex** numTex;
int numTexNum = 0;

#define STATE_TEX 10
StateTex* _sateTex;
StateTex** stateTex;
int stateTexNum = 0;

#define WEAK_TEX 4
WeakTex* _weakTex;
int weakTexNum = 0;

void GameManager::loadGamePop()
{
	iGraphics* g = iGraphics::share();

	//numTex
	_numTex = new NumberTex[NUM_TEX];
	for (int i = 0; i < NUM_TEX; i++)
	{
		NumberTex* nt = &_numTex[i];
		nt->init();
	}

	numTex = new NumberTex * [NUM_TEX];

	//stateTex
	StateTex* st = new StateTex[STATE_TEX];
	for (int i = 0; i < STATE_TEX; i++)
	{
		StateTex* s = &st[i];
		s->init();
	}

	_sateTex = st;
	stateTex = new StateTex * [NUM_TEX];

	//weakTex
	WeakTex* wt = new WeakTex[WEAK_TEX];
	for (int i = 0; i < WEAK_TEX; i++)
	{
		WeakTex* w = &wt[i];
		w->init();
	}
	
	_weakTex = wt;

	//ipopup
	gamePop = new iPopup();
	gamePop->style = iPopupStyleNone;
	gamePop->sp = iVector2DMake(DEV_WIDTH * .5f, DEV_HEIGHT * .5f);
	gamePop->ep = iVector2DMake(DEV_WIDTH *.5f, DEV_HEIGHT *.5f);

	endTurnBtn = new Botton();
	endTurnBtn->str = "Draw";
	endTurnBtn->relatePos = iVector2DMake(DEV_WIDTH - (125 + 145), DEV_HEIGHT - 97);
	endTurnBtn->relateSize = iVector2DMake(250, 70);
	endTurnBtn->idx = 0;
	endTurnBtn->initColor(0, 0, 1, 1);
	endTurnBtn->onClick = onClickEndTurnBtn;
	endTurnBtn->onMouseOverlap = onMouseOverlapEndTurnBtn;

	gamePop->addObject(endTurnBtn);

	fbar = new iPopupFillBar();
	fbar->relatePos = iVector2DMake(p->pos.x - 100 , p->pos.y - 100);
	fbar->relateSize = iVector2DMake(200, 30);
	fbar->initColor(1, 1, 1, 1);
	fbar->barR = 1; fbar->barG = 0; fbar->barB = 0; fbar->barA = 1;

	gamePop->addObject(fbar);

	iPopupTextBox* bt = new iPopupTextBox();
	bt->fontSize = 30;
	bt->setText("%d", p->block);
	bt->relatePos = iVector2DMake(fbar->relatePos.x - 45, fbar->relatePos.y);
	bt->relateSize = iVector2DMake(50, 50);
	bt->initColor(.5, .5, .5, 1);

	blockTextBox = bt;
	gamePop->addObject(bt);

	//enemyBar
	enemyFbar = new iPopupFillBar[3];

	for (int i = 0; i < 3; i++)
	{
		enemyFbar[i].initColor(1, 1, 1, 1);
		enemyFbar[i].barR = 1; 
		enemyFbar[i].barG = 0; 
		enemyFbar[i].barB = 0; 
		enemyFbar[i].barA = 1;
		enemyFbar[i].bShow = false;

		gamePop->addObject(&enemyFbar[i]);
	}

	topBar = new iPopupImage();
	iRect tbRt = { {0, 0} , {DEV_WIDTH , DEV_HEIGHT * .05f} };
	g->init(tbRt.size.x , tbRt.size.y);
	g->setColor(1, 1, 1, 1);
	g->fillRect(tbRt);
	g->setColor(1, 1, 1, 1);
	topBar->tex = g->getTexture();
	topBar->relatePos = iVector2DMake(0,0);
	topBar->relateSize = iVector2DMake(DEV_WIDTH,DEV_HEIGHT*.05f);
	topBar->initColor(.5, .5, .5, 1);

	gamePop->addObject(topBar);

	iPopupTextBox* tb = new iPopupTextBox();
	tb->fontSize = 50;
	tb->setText("%d/%d", p->cost, p->totalCost);
	tb->relatePos = iVector2DMake(deck->pos.x, deck->pos.y - 90);
	tb->relateSize = iVector2DMake(150, 50);
	tb->initColor(0, 0, 0, 1);

	costTextBox = tb;
	gamePop->addObject(tb);
}

void addNumberTex(iVector2D spawnP, int num)
{
	iGraphics* g = iGraphics::share();

	for (int i = 0; i < NUM_TEX; i++)
	{
		NumberTex* nt = &_numTex[i];
		if (!nt->active)
		{
			nt->pos = spawnP;
			if (nt->tex) delete nt->tex;
			//TODO : so slow to make texture
			g->setStringColor(1, 1, 1, 1);
			nt->tex = g->getStringTexInRect(100, 100, 0, "%d", num);
			g->setStringColor(1, 1, 1, 1);
			nt->force.x = 100.f *(random() % 2 ? 1 : -1);
			nt->active = true;
			nt->degree = 0;

			numTex[numTexNum] = nt;
			numTexNum++;
			break;
		}
	}

	return;
}

void paintNumTex(float dt)
{
	for (int i = 0; i < numTexNum; i++)
	{
		NumberTex* nt = numTex[i];

		if (nt->paint(dt))
		{
			nt->init();
			numTexNum--;
			numTex[i] = numTex[numTexNum];
			i--;
		}
	}
}

void addStateTex(iVector2D start, iVector2D end, const char* szFormat,...)
{
	char szText[255];
	formating_text(szText, szFormat);

	iGraphics* g = iGraphics::share();

	for (int i = 0; i < STATE_TEX; i++)
	{
		StateTex* st = &_sateTex[i];
		if (!st->active)
		{
			st->pos = start;
			st->start = start;
			st->end = end;
			if (st->tex) delete st->tex;
			//TODO : so slow to make texture
			g->setStringColor(1, 1, 1, 1);
			st->tex = g->getStringTexInRect(200, 100, 0, szText);
			g->setStringColor(1, 1, 1, 1);
			st->active = true;

			stateTex[stateTexNum] = st;
			stateTexNum++;
			break;
		}
	}

	return;
}

void paintStateTex(float dt)
{
	for (int i = 0; i < stateTexNum; i++)
	{
		StateTex* st = stateTex[i];

		if (st->paint(dt))
		{
			st->init();
			stateTexNum--;
			stateTex[i] = stateTex[stateTexNum];
			i--;
		}
	}
}

void addWeakTex(iVector2D upPos, iVector2D downPos, int* weak)
{
	for (int i = 0; i < WEAK_TEX; i++)
	{
		WeakTex* wt = &_weakTex[i];
		if (!wt->active)
		{
			iGraphics* g = iGraphics::share();
			iVector2D size = iVector2DMake(50, 50);
			g->init(size.x, size.y);
			g->setColor(.8f, .2f, .2f, 1.f);
			g->fillRect(iRect({ 0,0 }, size));
			g->setColor(1,1,1,1);
			g->setStringColor(1, 1, 1, 1);
			iVector2D pos = size * .5f;
			g->drawString(pos, VCENTER | HCENTER, "%d", *weak);
			g->setStringColor(1, 1, 1, 1);

			if (wt->tex) delete wt->tex;
			wt->weak = weak;
			wt->tex = g->getTexture();
			wt->active = true;
			wt->delta = 0.f;
			wt->upPos = upPos;
			wt->downPos = downPos;

			weakTexNum++;
			break;
		}
	}

	return;
}

void paintWeakTex(float dt)
{
	for (int i = 0; i < weakTexNum; i++)
	{
		WeakTex* wt = &_weakTex[i];
		wt->paint(dt);
	}
}

float delta = 0 , _delta= 2.f;
float toRate = 0;
int curCost = 0;
void GameManager::updateGamePop(float dt)
{
	//player hpbar
	if (fbar->rate != toRate)
	{
		delta += dt;
		float _r = delta / _delta;
		float r = linear(fbar->rate, toRate, _r);
		fbar->rate = r;
	}
	else
	{
		delta = 0;
	}

	toRate = (float)p->hp / (float)p->maxHp;

	//player block
	blockTextBox->setText("%d", p->block);

	//enemy hpbar
	int eNum = curStage->enemyNum;
	iVector2D offS = iVector2DMake(250/eNum, 30);
	iVector2D offP = iVector2DMakeZero;

	for (int i = 0; i < eNum; i++)
	{
		offP.x = curStage->enemy[i].pos.x - offS.x/2;
		offP.y = 
		(curStage->enemy[i].pos.y - curStage->enemy[i].size.y) - 50;

		enemyFbar[i].relatePos = offP;
		enemyFbar[i].relateSize = offS;
		enemyFbar[i].rate = 
		(float)curStage->enemy[i].hp / (float)curStage->enemy[i].maxHp;
		enemyFbar[i].bShow = true;
	}

	if (playerTurn) endTurnBtn->str = "End Turn";
	else endTurnBtn->str = "Enemy Turn";

	//costTextBar
	if (curCost != p->cost)
	{
		costTextBox->setText("%d/%d", p->cost, p->totalCost);
	}
}

void GameManager::freeGamePop()
{
	for (int i = 0; i < NUM_TEX; i++)
		if(_numTex[i].tex) delete _numTex[i].tex;
	delete[] _numTex;
	delete[] numTex;

	for (int i = 0; i < STATE_TEX; i++)
		if (_sateTex[i].tex) delete _sateTex[i].tex;
	delete[] _sateTex;
	delete[] stateTex;

	for (int i = 0; i < WEAK_TEX; i++)
		if (_weakTex[i].tex) delete _weakTex[i].tex;
	delete[] _weakTex;

	delete gamePop;
	delete endTurnBtn;
	delete fbar;
	delete[] enemyFbar;
	delete topBar;
	delete costTextBox;
	delete blockTextBox;
}








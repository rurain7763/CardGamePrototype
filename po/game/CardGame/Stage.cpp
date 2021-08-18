#include"Stage.h"
#include"iStd.h"
#include"Enemy.h"

#define STAGE_RED 1,0,0,1
#define STAGE_GREEN 0,1,0,1
#define STAGE_BLUE 0,0,1,1

#define BOTTON_SIZE_X 30
#define BOTTON_SIZE_Y 30

static iTexture** stageTex; // 0-normal , 1-shop , 2-event , 3-named , 4-boss
static iPopup* stagePop;

Stage::Stage()
{
	sn = 0;
	for (int i = 0; i < 3; i++) next[i] = -1;
	connected = false;
	idx = 0;

	type = StageTypeMax;
	enemy = nullptr;
	enemyNum = 0;

#if 0
	nextStage = NULL;
	nextStageNum = 0;
	enable = false;
#endif
}

Stage::~Stage()
{
	if (enemy) delete enemy;

#if 0
	if (nextStage) delete nextStage;
#endif
}

#if 0
//TODO : must move variable somewhere
static Stage* testStage;
static int* heightInfo;
static iPopupScrollImage* scrollImg;
static Botton** stagePopBtn;
static int stageBtnNum;

void cbScrollPopBefore(void* parm)
{
	Botton* obj = (Botton*)parm;

	Stage* s = &testStage[obj->idx];

	for (int i = 0; i < s->nextStageNum; i++)
	{
		Stage* next = s->nextStage[i];

		iVector2D start = obj->relatePos + obj->relateSize*.5f;

		Botton* btn = stagePopBtn[next->idx];
		iVector2D end = btn->relatePos + btn->relateSize*.5f;

		drawLine(start, end);
	}
}

void onClickBtn(void* parm)
{
	Botton* b = (Botton*)parm;
	//toStage = b->idx;
	//stagePop->show(false);

	Stage* s = &testStage[b->idx];
	printf("type :%d enable: %d connected : %d curIdx : %d", s->type, s->enable, s->connected, s->idx);

	for (int i = 0; i < s->nextStageNum; i++)
	{
		Stage* next = s->nextStage[i];
		printf("%d\n", next->idx);
	}
}

void _loadStage()
{
	//====================================================load texture
	iTexture** st = new iTexture*[5];

	iGraphics* g = iGraphics::share();
	iVector2D size = iVector2DMake(BOTTON_SIZE_X, BOTTON_SIZE_Y);
	
	for (int i = 0; i < 5; i++)
	{
		iTexture* t = new iTexture();
		g->init(size.x, size.y);
		if (i == 0) g->setColor(1, 1, 1, 1);
		else if (i == 1) g->setColor(0, 0, 0, 1);
		else if (i == 2) g->setColor(STAGE_GREEN);
		else if (i == 3) g->setColor(STAGE_BLUE);
		else if (i == 4) g->setColor(STAGE_RED);
		g->fillRect( 0, 0, size.x, size.y);
		g->setColor(1, 1, 1, 1);
		t = g->getTexture();
		st[i] = t;
	}

	stageTex = st;

	//====================================================make stage
	int stageRandom[] = { 0,0,0,0,0,0,0,0,0,0,1,2,3 };

	iArray stages;
	int maxStageNum = 40;
	Stage* _stage = new Stage[maxStageNum];
	int off = 0;
	int height = 0;
	int seed = 4;
	int rnd;
	int i, j;

	int heightInfoSize = 10;
	heightInfo = new int[heightInfoSize];

	while (1)
	{
		if (height == 0) rnd = seed;
		else rnd = 2 + random() % 3;

		if (off + rnd >= maxStageNum) break;

		for (i = 0; i < rnd; i++)
		{
			Stage* s = &_stage[off+i];
			int eRand = 1 + random() % 3;
			s->enemy = new Enemy[eRand];
			//TODO : set enemy
			s->enemyNum = eRand;
			s->type = (StageType)(stageRandom[random() % 13]);
			s->enable = true;
			s->height = height;
			s->idx = off + i;
			if (height == 0) s->connected = true;
		}

		if (stages.num != 0)
		{
			for (i = 0; i < stages.num ; i++)
			{
				Stage* s = (Stage*)stages.at(i);

				//stages.num > rnd
				if (random() % 2) //connect 2 stage
				{
					s->nextStage = new Stage * [2];
					Stage* t = &_stage[off + i];

					if (t->enable)
					{
						for (j = 0; j < 2; j++)
						{
							t = &_stage[(off + j) + i];
							if (!t->enable) continue;
							s->nextStage[s->nextStageNum] = t;
							s->nextStageNum++;
							t->connected = true;
						}
					}
					else
					{
						for (j = i; j > -1; j--)
						{
							t = &_stage[off + j];
							if (t->enable) break;
						}

						s->nextStage[s->nextStageNum] = t;
						s->nextStageNum++;
						t->connected = true;
					}
				}
				else //connect 1 stage
				{
					s->nextStage = new Stage * [1];
					Stage* t = &_stage[off + i];
					if (t->enable)
					{
						s->nextStage[s->nextStageNum] = t;
						s->nextStageNum++;
						t->connected = true;
					}
					else
					{
						for (j = i; j > -1; j--)
						{
							t = &_stage[off + j];
							if (t->enable) break;
						}

						s->nextStage[s->nextStageNum] = t;
						s->nextStageNum++;
						t->connected = true;
					}
				}
			}

			stages.clear();
		}

		int num = rnd;
		for (i = 0; i < num; i++)
		{
			if (!_stage[off + i].connected)
			{
				rnd--;
				continue;
			}

			stages.push_back(&_stage[off + i]);
		}
		off += num;

		if (heightInfoSize == height)
		{
			int* c = new int[heightInfoSize + 10];
			memcpy(c, heightInfo, sizeof(int) * heightInfoSize);
			delete[] heightInfo;
			heightInfo = c;
			heightInfoSize += 10;
		}

		heightInfo[height] = rnd;
		height++;
	}

	testStage = _stage;

	//=======================================================make stage pop
	iPopup* pop = new iPopup();
	pop->style = iPopupStyleMove;
	pop->sp = iVector2DMake(-DEV_WIDTH, DEV_HEIGHT / 2);
	pop->ep = iVector2DMake(DEV_WIDTH / 2, DEV_HEIGHT / 2);
	pop->_delta = .5f;
	
	iPopupScrollImage* si = new iPopupScrollImage(iVector2DMake(0, 0),
												  iVector2DMake(DEV_WIDTH, DEV_HEIGHT), 
												  iVector2DMake(DEV_WIDTH, DEV_HEIGHT * 3.f));
	
	scrollImg = si;
	si->before = cbScrollPopBefore;

	iVector2D center = iVector2DMake(DEV_WIDTH *.5f, DEV_HEIGHT * 3.f - 40);
	iVector2D btnSize = iVector2DMake(BOTTON_SIZE_X, BOTTON_SIZE_Y);
	iVector2D interVal = iVector2DMake(80,120);
	iVector2D interRng = iVector2DMake(20,20);

	Botton** instance = new Botton*[maxStageNum];
	stageBtnNum = 0;

	for (int i = 0; i < maxStageNum; i++)
	{
		Stage* s = &testStage[i];
		if (!s->connected) continue;

		Botton* b = new Botton();
		int num = heightInfo[s->height];
		int startX = center.x - (num * btnSize.x + (num - 1) * interVal.x) / 2;

		b->idx = s->idx;
		b->relatePos = iVector2DMake(startX + i % num * (btnSize.x + interVal.x), 
								     center.y - s->height * (btnSize.y + interVal.y) - random()%(int)interRng.y);
		b->relateSize = btnSize;
		b->tex = stageTex[s->type];
		b->onClick = onClickBtn;
		b->onMouseOverlap = NULL;
		b->active = true;

		si->addObject(b);

		instance[i] = b;
		stageBtnNum = i;
	}

	stageBtnNum++;
	stagePopBtn = new Botton * [stageBtnNum];
	memcpy(stagePopBtn, instance, sizeof(Botton*)*stageBtnNum);
	delete[] instance;
	delete heightInfo;

	pop->addObject(si);
	stagePop = pop;
}

void _freeStage()
{
	for (int i = 0; i < 5; i++) 
		delete stageTex[i];
	delete[] stageTex;

	delete testStage;

	delete stagePop;
	delete scrollImg;
	for (int i = 0; i < stageBtnNum; i++)
		delete stagePopBtn[i];
	delete stagePopBtn;
}

void _drawStage(float dt)
{
	stagePop->paint(dt);
}

void _showStage(bool show)
{
	stagePop->show(show);
}

#endif
#pragma once
#include"GameAttribute.h"
#include"iVector2D.h"

class Enemy;

enum StageType
{
	StageTypeBattle = 0,
	StageTypeShop,
	StageTypeEvent,
	StageTypeNamed,

	StageTypeMax,
	StageTypeBoss
};

class Stage
{
public:
	Stage();
	virtual ~Stage();

public:
	int sn;
	int next[3];
	int idx;
	bool connected;

#if 0
	int height;
	Stage** nextStage;
	int nextStageNum;
	bool enable;
#endif

public:
	StageType type;
	Enemy* enemy;
	int enemyNum;
};

#if 0
void _loadStage();
void _freeStage();
void _drawStage(float dt);
void _showStage(bool show);
#endif

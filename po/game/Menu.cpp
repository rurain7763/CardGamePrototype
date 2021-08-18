#include"Menu.h"
#include"Loading.h"
#include"Proc.h"
#include"iStd.h"

void loadMenu()
{
	loadMainMenu();
	showMainMenu(true);
	loadSubMenuExit();
}

void drawMenu(float dt)
{
	drawMainMenu(dt);
	drawSubMenuExit(dt);
}

void freeMenu()
{
	freeMainMenu();
	freeSubMenuExit();
}

void keyMenu()
{
}

//-------------------------------------MainMenu
static iPopup* mainMenu;
static Botton* mainMenuBtn;
static iPopupSlideImage* mainMenuSlideImg;
const char* playerName;
int curIdx;

//main menu src
static iTexture* mainTex;

void onClickMenuBtn(void* parm)
{
	Botton* b = (Botton*)parm;

	if (b->idx == 0) // game start
	{
		setLoading(SCENE_PROC, freeMenu, loadProc);		
	}
	else if (b->idx == 1) //option
	{

	}
	else if (b->idx == 2) // game exit
	{
		showSubMenuExit(true);
	}
}

void onMouseOverlapMenuBtn(void* parm)
{
	Botton* b = (Botton*)parm;
	b->setColor(1, 0, 0, 1);
}

void loadMainMenu()
{
	mainMenu = new iPopup();
	mainMenu->style = iPopupStyleAlpha;
	mainMenu->sp = iVector2DMakeZero;
	mainMenu->ep = iVector2DMake(DEV_WIDTH * .5f, DEV_HEIGHT *.5f);
	mainMenu->_delta = 1.f;

	iPopupSlideImage* si = new iPopupSlideImage();
	mainTex = new iTexture[3];
	for (int i = 0; i < 3; i++)
	{
		mainTex[i].createImage("assets/Menu/SilentBG.jpg");
	}

	si->relatePos = iVector2DMake(DEV_WIDTH * .5f, DEV_HEIGHT *.5f);
	si->relateSize = iVector2DMake(DEV_WIDTH, DEV_HEIGHT);
	si->addTex(mainTex, 3);
	si->initColor(1, 1, 1, 1);

	mainMenu->addObject(si);
	mainMenuSlideImg = si;

	mainMenuBtn = new Botton[3];

	const char* btnStr[3] =
	{
		"Game start",
		"Option",
		"Exit"
	};

	for (int i = 0; i < 3; i++)
	{
		Botton* b = &mainMenuBtn[i];

		b->str = btnStr[i];
		b->idx = i;
		b->relatePos = iVector2DMake(DEV_WIDTH / 4 * 3, DEV_HEIGHT / 2 + 60 * i);
		b->relateSize = iVector2DMake(300, 50);
		b->initColor(1, 1, 1, 1);
		b->onClick = onClickMenuBtn;
		b->onMouseOverlap = onMouseOverlapMenuBtn;

		mainMenu->addObject(b);
	}

	curIdx = 0;
	playerName = "Silent";
}

void freeMainMenu()
{
	delete[] mainTex;
	delete mainMenuSlideImg;
	delete[] mainMenuBtn;
	delete mainMenu;
}

void drawMainMenu(float dt)
{
	mainMenu->paint(dt);

	iPopupSlideImage* si = mainMenuSlideImg;
	if(curIdx != si->curIdx)
	{
		if (si->curIdx == 0)
		{
			playerName = "Silent";
		}
		else if (si->curIdx == 1)
		{
			playerName = "Worrior";
		}
		else if (si->curIdx == 2)
		{
			playerName = "Archor";
		}

		curIdx = mainMenuSlideImg->curIdx;
	}
}

void showMainMenu(bool s)
{
	mainMenu->show(s);
}

//-------------------------------------SubMenuExit
static iPopup* subMenuExit;
static Botton* subMenuExitBtn;
static iPopupImage* subMenuExitImg;
static iPopupTextBox* subMenuExitText;

void onClickSubMenuExitBtn(void* parm)
{
	Botton* b = (Botton*)parm;

	if (b->idx == 0) // Yes
	{
		run = false;
	}
	else if (b->idx == 1) // No
	{
		showSubMenuExit(false);
	}
}

void onMouseOverlapSubMenuExitBtn(void* parm)
{
	Botton* b = (Botton*)parm;
	b->setColor(1, 0, 0, 1);
}

void loadSubMenuExit()
{
	subMenuExitImg = new iPopupImage();
	subMenuExitBtn = new Botton[2];

	subMenuExit = new iPopup();
	subMenuExit->style = iPopupStyleMove;
	subMenuExit->sp = iVector2DMake(-DEV_WIDTH, DEV_HEIGHT / 2);
	subMenuExit->ep = iVector2DMake(DEV_WIDTH / 2, DEV_HEIGHT / 2);
	subMenuExit->_delta = .5f;

	iPopupImage* si = new iPopupImage();
	iGraphics* g = iGraphics::share();
	iVector2D size = iVector2DMake(DEV_WIDTH * .55, DEV_HEIGHT * .55f);
	g->init(size.x, size.y);
	g->setColor(.5, .5, .5, 1);
	g->fillRect(iRect({ 0,0 }, size));
	g->setColor(1, 1, 1, 1);
	si->tex = g->getTexture();
	si->initColor(1, 1, 0, .7f);
	si->relatePos = iVector2DMake(DEV_WIDTH *.25, DEV_HEIGHT *.25f);
	si->relateSize = iVector2DMake(DEV_WIDTH*.5f, DEV_HEIGHT*.5f);

	subMenuExit->addObject(si);
	subMenuExitImg = si;

	const char* btnStr[2] =
	{
		"Yes",
		"No"
	};

	iVector2D off = iVector2DMake(DEV_WIDTH * .25, DEV_HEIGHT * .25f);

	for (int i = 0; i < 2; i++)
	{
		Botton* b = &subMenuExitBtn[i];

		b->str = btnStr[i];
		b->idx = i;
		b->relatePos = iVector2DMake((off.x+150) + 200 * i, off.y + 300);
		b->relateSize = iVector2DMake(150, 30);
		b->initColor(1, 1, 1, 1);
		b->onClick = onClickSubMenuExitBtn;
		b->onMouseOverlap = onMouseOverlapSubMenuExitBtn;

		subMenuExit->addObject(b);
	}

	iPopupTextBox* tb = new iPopupTextBox();

	tb->fontSize = 30;
	tb->initColor(1, 0, 0, 1);
	tb->relatePos = iVector2DMake(0,0);
	tb->relateSize = iVector2DMake(DEV_WIDTH, DEV_HEIGHT);
	tb->setText("Do you want exit?");
	subMenuExit->addObject(tb); 
	subMenuExitText = tb;
}

void freeSubMenuExit()
{
	delete subMenuExit;
	delete[] subMenuExitBtn;
	delete subMenuExitImg;
	delete subMenuExitText;
}

void drawSubMenuExit(float dt)
{
	subMenuExit->paint(dt);
}

void showSubMenuExit(bool s)
{
	subMenuExit->show(s);
}



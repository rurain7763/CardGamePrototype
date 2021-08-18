#include"Game.h"
#include"Loading.h"
#include"Menu.h"
#include"Proc.h"
#include"iStd.h"

#define MENU_TEST
void loadGame()
{
#ifdef MENU_TEST	
	loadLoading();
	setLoading(SCENE_MENU,freeGame,loadMenu);
#endif

#ifdef PROC_TEST	
	loadLoading();
	setLoading(SCENE_PROC, freeGame, loadProc);
#endif
}

void drawGame(float dt)
{
	if(!drawLoading(dt)) return;

	switch (sS)
	{
	case SCENE_MENU: drawMenu(dt); break;
	case SCENE_PROC: drawProc(dt); break;
	}

	keyGame();
}

void freeGame()
{
	switch (sS)
	{
	case SCENE_MENU: freeMenu(); break;
	case SCENE_PROC: freeProc(); break;
	}

	if(!run) freeLoading();
}

void keyGame()
{
	switch (sS)
	{
	case SCENE_MENU: keyMenu(); break;
	case SCENE_PROC: keyProc(); break;
	}
}




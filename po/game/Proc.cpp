#include"Proc.h"
#include"iStd.h"
#include"CardGame/GameManager.h"

GameManager* gm;
iTexture* bg;

void loadProc()
{
	randomSeed(time(NULL));

	bg = new iTexture();
	gm = new GameManager();
	bg->createImage("assets/Proc/dungeonBG.jpg");	
	gm->start();
}

void drawProc(float dt)
{
	iVector2D pos = iVector2DMake(0, 0);
	drawImage(bg, pos, BOTTOM | RIGHT,
			  iVector2DMake(0, 0), iVector2DMake(bg->width, bg->height),
			  iVector2DMake((float)DEV_WIDTH / bg->width, (float)DEV_HEIGHT / bg->height),
			  2, 0);
	
	gm->update(dt);
}

void freeProc()
{
	delete bg;
}

void keyProc()
{

}

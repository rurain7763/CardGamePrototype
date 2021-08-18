#include"Loading.h"
#include"iStd.h"
#include"iWindows.h"

unsigned int sS = SCENE_NONE;
unsigned int toScene;
SCENE_METHOD lS;
SCENE_METHOD fS;
float curT;
static float freeDelta;

iAnimation* loadingAnim;
iTexture** loadingTex;

void loadLoading()
{
	loadingAnim = new iAnimation();
	loadingTex = sliceTexture(12, 4, "assets/loading_1.png");

	for (int i = 0; i < 48; i++)
		loadingAnim->addTexture(loadingTex[i]);
	
	loadingAnim->repeat = true;
}

void freeLoading()
{
	for (int i = 0; i < 48; i++)
		delete loadingTex[i];

	delete loadingTex;
	delete loadingAnim;
}

void setLoading(unsigned int tS,SCENE_METHOD f,SCENE_METHOD l)
{
	lS = l;
	fS = f;
	toScene = tS;
	curT = 0.f;
	freeDelta = 0.f;
}

bool drawLoading(float dt)
{
	if (curT == -1) return true;

#if 1 // TODO : use thread
	if (loadingAnim->curFrame >= 20)
	{
		if (fS)
		{
			fS();
			fS = NULL;
		}

		freeDelta += dt;

		if (freeDelta >= 1.f)
		{
			lS();
			lS = NULL;
			sS = toScene;
			curT = -1;
			loadingAnim->curFrame = 0;
			freeDelta = 0;
			return true;
		}
	}
#endif

	setRGBA(.3, 0, .3, 1);
	fillRect(iRect({ 0,0 }, { DEV_WIDTH,DEV_HEIGHT }));
	setRGBA(1, 1, 1, 1);
	loadingAnim->start(dt, iVector2DMake(DEV_WIDTH / 2, DEV_HEIGHT / 2),
					   iVector2DMake(1, 1), VCENTER | HCENTER);

	return false;
}


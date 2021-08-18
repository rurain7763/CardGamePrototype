#pragma once
#include"iType.h"

class iAnimation;
class iTexture;

extern unsigned int sS;

void loadLoading();
void freeLoading();
void setLoading(unsigned int tS, SCENE_METHOD f, SCENE_METHOD l);
bool drawLoading(float dt);
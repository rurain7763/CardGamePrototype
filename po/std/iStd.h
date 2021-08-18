#pragma once

#include"CardGame/GameAttribute.h"
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <map>

#include"iType.h"

#include"iOpenGL.h"
#include"iFBO.h"

#include"Game.h"
#include"iFPS.h"
#include"iVector2D.h"
#include"iRect.h"
#include"iInputMangement.h"
#include"iTexture.h"
#include"iGraphics.h"
#include"iArray.h"
#include"iAnimation.h"
#include"iPopup.h"
#include"iString.h"
#include"iTextReader.h"

//LANGUAGE_CODE
#define US 0
#define KR 2
#define JP 4
#define LANGUAGE US

//DEVSIZE
#define DEV_WIDTH 1280
#define DEV_HEIGHT 720

#define formating_text(szText,szFormat)  va_list list; \
										 va_start(list, szFormat); \
										 vsnprintf(szText, sizeof(szText), szFormat, list); \
										 va_end(list)

extern bool run;

int charToint(const char* s);
float _cos(int degree);
float _sin(int degree);
float clamp(float v, float l, float h);
float linear(float s, float e, float r);
float easeIn(float s, float e, float r);
int nextPot(int v);
void randomSeed(unsigned int seed);
int random();
char* getFormatingText(const char* szFormat, ...);

void setLineWidth(float width);
void drawLine(iVector2D s, iVector2D e);
void drawLine(float sx, float sy, float ex, float ey);

void drawRect(iRect rt);
void fillRect(iRect rt);

void setRGBA(const float r, const float g, const float b, const float a);
void getRGBA(float& r, float& g, float& b, float a);
void drawImage( iTexture* tex, iVector2D pos, int anc,
				iVector2D iPos, iVector2D iSize, iVector2D size,
				int rXYZ=0, float degree=0, int reverse=0);

iTexture** sliceTexture(int numX, int numY, const char* szForamat, ...);

extern iRect viewPort;
void getViewPort(int width , int height);
iVector2D coordinate(iVector2D pos);
char* readFile(int& fileLen , const char* filePath, ...);
void saveFile(char* buf, int len, const char* filePath, ...);

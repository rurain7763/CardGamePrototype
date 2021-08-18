#pragma once
#include"iVector2D.h"
#include"iRect.h"
#include"iString.h"

typedef void (*AFTER_DRAW_OBJECT)(void* parm);
typedef void (*BEFORE_DRAW_OBJECT)(void* parm);

class iArray;
class iTexture;
class iFBO;
class iPopup;

//-------------------------------iPopupObject
class iPopupObject
{
public:
	iPopupObject();
	virtual ~iPopupObject();

	void initColor(float r, float g, float b, float a);
	void setColor(float r, float g, float b, float a);
	virtual void paint(float dt,iVector2D off) = 0;

public:
	float r, g, b, a;
	float br, bg, bb, ba;
	int idx;
	bool bShow;
	bool active;
	iVector2D relatePos;
	iVector2D relateSize;
};

//-------------------------------iPopupBotton
typedef void (*OnClick)(void* parm);
typedef void (*OnMouseBottonOverlap)(void* parm);
class Botton : public iPopupObject
{
public:
	Botton();
	virtual ~Botton();

	virtual void paint(float dt,iVector2D off);

private:
	iString curStr;
	int strW; int strH;
	iTexture* strTex;

public:
	iTexture* tex;
	iString str;
	OnClick onClick;
	OnMouseBottonOverlap onMouseOverlap;
};

//-------------------------------iPopupImage
class iPopupImage : public iPopupObject
{
public:
	iPopupImage();
	virtual ~iPopupImage();

	virtual void paint(float dt, iVector2D off);

public:
	iTexture* tex;
};

//-------------------------------iPopupTextBox
class iPopupTextBox : public iPopupObject
{
public:
	iPopupTextBox();
	virtual ~iPopupTextBox();

	void setText(const char* szFormat,...);
	void setText(const iString& str);
	virtual void paint(float dt, iVector2D off);

private:
	iTexture* strTex;
	iString str;
	iString curStr;
	float strW, strH;

public:
	float fontSize;
	float backR, backG, backB, backA;
};

//-------------------------------iPopupFillBar
class iPopupFillBar : public iPopupObject
{
public:
	iPopupFillBar();
	virtual ~iPopupFillBar();

	virtual void paint(float dt, iVector2D off);

public:
	float rate; // 0~1
	float barR, barG, barB, barA;
};

//-------------------------------iPopupSlideImage
enum iPopupSlideImageStyle
{
	iPopupSlideImageStyleVertical = 0,
	iPopupSlideImageStyleHorizontal
};

class iPopupSlideImage : public iPopupObject
{
public:
	iPopupSlideImage();
	virtual ~iPopupSlideImage();

	void addTex(iTexture* tex , int num = 1);
	virtual void paint(float dt, iVector2D off);

private:
	iArray* tex;
	iTexture* slideTex;

	iVector2D off;
	iVector2D minPos, maxPos;
	iVector2D prevP;

	unsigned long prevTime;
	iVector2D dir;
	float force;
	bool touching;

public:
	iPopupSlideImageStyle style;
	int curIdx;
};

//-------------------------------iPopupScrollBar : Vertical
class iPopupScrollBar : public iPopupObject
{
public:
	iPopupScrollBar(float x ,float y , int w, int h);
	virtual ~iPopupScrollBar();

	virtual void paint(float dt, iVector2D off);

private:
	iTexture* boxTex;

public:
	iVector2D maxShowSize;
	iVector2D contentsSize;
	float rate;
};

//-------------------------------iPopupScrollImage
class iPopupScrollImage : public iPopupObject
{
public:
	iPopupScrollImage(iVector2D pos, iVector2D maxSize, iVector2D contentsSize);
	virtual ~iPopupScrollImage();

	void addObject(iPopupObject* obj);
	virtual void paint(float dt, iVector2D off);
	
private:
	iArray* objArr;
	iTexture* texForFBO;

	iPopupScrollBar* scrollBar;
	iTexture* tex;

	iVector2D off;
	iVector2D minPos;
	iVector2D maxPos;

	float force;
	float prevTime;
	iVector2D prevP;
	iVector2D dir;

public:
	BEFORE_DRAW_OBJECT before;
	AFTER_DRAW_OBJECT after;
	float rate;
};

//-------------------------------iPopup
enum iPopupStat
{
	iPopupStatOpen =0,
	iPopupStatProc,
	iPopupStatClose
};

enum iPopupStyle
{
	iPopupStyleNone = 0,
	iPopupStyleAlpha,
	iPopupStyleMove,
	iPopupStyleZoom
};

typedef void (*IPOPUP_STAT_EVENT)(iPopupStat stat);

class iPopup
{
public:
	iPopup();
	virtual ~iPopup();

	void addObject(iPopupObject* obj);
	void show(bool s);
	void paint(float dt);

public:
	iTexture* texForPop;
	iArray* objects;

	bool bShow;
	IPOPUP_STAT_EVENT event;
	iPopupStyle style;
	iPopupStat stat;
	iVector2D sp, ep;
	float delta, _delta;
};
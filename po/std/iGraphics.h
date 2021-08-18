#pragma once
#include"iVector2D.h"
#include"iWindows.h"

class iRect;
class iTexture;

class iBitmap
{
public:
	iBitmap();
	virtual ~iBitmap();

public:
	Bitmap* bmp;
	int width;
	int height;
};

class iGraphics
{
public:
	iGraphics();
	virtual ~iGraphics();

	void start(HDC hdc);

	void init(int w,int h);
	iTexture* getTexture();

	void setColor(float r, float g, float b, float a);
	void drawLine(iVector2D s, iVector2D e);
	void drawLine(float sx, float sy, float ex, float ey);
	void drawRect(iRect rt);
	void fillRect(iRect rt);
	void fillRect(float top, float left, float right, float bottom);

	iBitmap** sliceBitmap(int numX, int numY, const char* szFormat, ...);

	void drawImage(iBitmap* tex, iVector2D pos, int anc,
		iVector2D iPos, iVector2D iSize, iVector2D size,
		int rXYZ, float degree, int reverse);

	void setFontName(const char* sn);
	void setStringColor(float r, float g, float b, float a);
	static iGraphics* share();
	void checkFontFamily(void* fontFamily, int& fs);
	iRect stringRect(const char* szText);
	void drawString(iVector2D& pos, int anc, const char* szFormat, ...);
	void drawString(void* g, iVector2D& pos, int anc, const char* szFormat, ...);
	iTexture* getStringTexInRect(int w, int h, int inter, const char* szFormat, ...);
	iBitmap* getStringBmpInRect(int w, int h, int inter, const char* szFormat, ...);

private:
	static iGraphics* s;
public:
	Bitmap* _bmp;
	Graphics* _bmpG;

	ULONG_PTR token;
	Bitmap* bmp;
	Graphics* bB;
	Graphics* fB;

	float pr, pg, pb, pa;
	float pw;

	char* stringName;
	void* bmpForString;
	void* graphicsForString;
	void* brush;
	float ba, br, bg, bb;
	int fontSize;
	int borderSize;
};

#include"iStd.h"
#include"iWindows.h"

bool run = true;

int charToint(const char* s)
{
	int len = strlen(s);
	int p = 1;
	int r = 0;

	for (int i = len-1; i > -1; i--)
	{
		r += (s[i] - 48) * p;
		p *= 10;
	}

	return r;
}

float _cos(int degree)
{
	return cos(degree * M_PI / 180.f);
}

float _sin(int degree)
{
	return sin(degree * M_PI / 180.f);
}

float clamp(float v, float l, float h)
{
	if (v > l && v < h) return v;
	return v <= l ? l : h;
}

float linear(float s,float e,float r)
{
	r = clamp(r, 0, 1);
	return s + (e - s) * r;
}

float easeIn(float s, float e, float r)
{
	r = clamp(r, 0, 1);
	return s+(e-s)*_sin(90*r);
}

int nextPot(int v)
{
	v = v - 1;
	v = v | (v >> 1);
	v = v | (v >> 2);
	v = v | (v >> 4);
	v = v | (v >> 8);
	v = v | (v >> 16);

	return v+1;
}

void randomSeed(unsigned int seed)
{
	srand(seed);
}

int random()
{
	return rand();
}

char* getFormatingText(const char* szFormat,...)
{
	char* szText = new char[1024];
	formating_text(szText, szFormat);

	return szText;
}

static float _r=1.f, _g=1.f, _b=1.f, _a=1.f;
static float _lineWidth = 10.f;

void setLineWidth(float width)
{
	_lineWidth = width;
}

void drawLine(iVector2D s, iVector2D e)
{
	glLineWidth(_lineWidth);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	iVector2D p[2] =
	{
		s , e
	};

	float c[4][2] =
	{
		1,1,1,1, 1,1,1,1
	};

	glVertexPointer(2, GL_FLOAT, sizeof(iVector2D), p);
	glColorPointer(4, GL_FLOAT, sizeof(float) * 4, c);

	glDrawArrays(GL_LINES, 0, 2);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void drawLine(float sx, float sy, float ex, float ey)
{
	glLineWidth(_lineWidth);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	iVector2D p[2] =
	{
		{sx,sy} , {ex,ey}
	};

	float c[4][2] =
	{
		1,1,1,1, 1,1,1,1
	};

	glVertexPointer(2, GL_FLOAT, sizeof(iVector2D), p);
	glColorPointer(4, GL_FLOAT, sizeof(float) * 4, c);

	glDrawArrays(GL_LINES, 0, 2);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void drawRect(iRect rt)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	iVector2D p[4] =
	{
		{rt.origin.x,rt.origin.y} ,
		{rt.origin.x + rt.size.x,rt.origin.y},
		{rt.origin.x,rt.origin.y + rt.size.y},
		{rt.origin.x + rt.size.x,rt.origin.y + rt.size.y}
	};

	float c[4][4] =
	{
		{1,1,1,1}, {1,1,1,1},
		{1,1,1,1}, {1,1,1,1}
	};

	glVertexPointer(2, GL_FLOAT, sizeof(float) * 2, p);
	
	unsigned char indices[6] = { 0,1,2,2,1,3 };
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_BYTE,indices);

	glDisableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
}

void fillRect(iRect rt)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	iVector2D p[4] =
	{
		{rt.origin.x,rt.origin.y} ,	
		{rt.origin.x+rt.size.x,rt.origin.y},
		{rt.origin.x,rt.origin.y+rt.size.y},
		{rt.origin.x+rt.size.x,rt.origin.y+rt.size.y}
	};

	float c[4][4] =
	{
		{_r,_g,_b,_a}, {_r,_g,_b,_a},
		{_r,_g,_b,_a}, {_r,_g,_b,_a}
	};

	glVertexPointer(2, GL_FLOAT, sizeof(float) * 2, p);
	glColorPointer(4, GL_FLOAT, sizeof(float) * 4, c);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
}

void setRGBA(const float r, const float g, const float b, const float a)
{
	_r = r; _g = g; _b = b; _a = a;
}

void getRGBA(float& r, float& g, float& b, float a)
{
	r = _r; g = _g; b = _b; a = _a;
}

void drawImage(iTexture* tex, iVector2D pos, int anc,
	iVector2D iPos, iVector2D iSize, iVector2D size,
	int rXYZ, float degree, int reverse)
{
	float w = iSize.x * size.x, h = iSize.y * size.y;

	switch (anc)
	{
	case VCENTER | HCENTER:		pos.x -= w / 2; pos.y -= h / 2;		break;
	case HCENTER | BOTTOM:		pos.x -= w / 2;						break;
	case HCENTER | TOP:			pos.x -= w / 2; pos.y -= h / 2;		break;
	case VCENTER | RIGHT:		pos.x += w / 2; pos.y -= h / 2;		break;
	case VCENTER | LEFT:		pos.x -= w / 2; pos.y -= h / 2;		break;
	case TOP | LEFT:			pos.x -= w;		pos.y -= h;			break;
	case TOP | RIGHT:			pos.y -= h;							break;
	case BOTTOM | LEFT:			pos.x -= w;							break;
	case BOTTOM | RIGHT:											break;
	}

	iVector2D p[4] =
	{
		{pos.x,pos.y},
		{pos.x + w,pos.y},
		{pos.x,pos.y + h},
		{pos.x + w,pos.y + h}
	};

	if (reverse == REVERSE_NONE) {}
	else if (reverse == REVERSE_WIDTH)
	{
		float x = p[0].x;
		p[0].x = p[1].x;
		p[1].x = x;

		x = p[2].x;
		p[2].x = p[3].x;
		p[3].x = x;
	}
	else if (reverse == REVERSE_HEIGHT)
	{
		float x = p[0].y;
		p[0].y = p[2].y;
		p[2].y = x;

		x = p[1].y;
		p[1].y = p[3].y;
		p[3].y = x;
	}
	
	//TODO : issue - x,y rotate don't work when reverse height
	if (rXYZ == 0)
	{
		p[0].y =
		p[1].y = pos.y + h / 2 - h / 2 * _cos(degree);
		p[2].y =
		p[3].y = pos.y + h / 2 + h / 2 * _cos(degree);
	}
	else if (rXYZ == 1)
	{
		p[0].x =
		p[2].x = pos.x + w / 2 - w / 2 * _cos(degree);
		p[1].x =
		p[3].x = pos.x + w / 2 + w / 2 * _cos(degree);
	}
	else //if (rXYZ == 2)
	{
		iVector2D c = iVector2DMake(pos.x + w / 2, pos.y + h / 2);

		for (int i = 0; i < 4; i++)
			p[i] = iVector2DRotate(p[i], c, degree);
	}

	iVector2D uv[4] =
	{
		{iPos.x / tex->potWidth,iPos.y / tex->potHeight},
		{(iPos.x + iSize.x) /tex->potWidth , iPos.y / tex->potHeight },
		{iPos.x / tex->potWidth, (iPos.y+iSize.y) / tex->potHeight},
		{(iPos.x + iSize.x) / tex->potWidth,(iPos.y + iSize.y) / tex->potHeight}
	};

	float c[4][4] =
	{
		_r,_g,_b,_a,
		_r,_g,_b,_a,
		_r,_g,_b,_a,
		_r,_g,_b,_a
	};

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, tex->imgID);
	glVertexPointer(2, GL_FLOAT, sizeof(iVector2D), p);
	glColorPointer(4, GL_FLOAT, sizeof(float) * 4, c);
	glTexCoordPointer(2, GL_FLOAT, sizeof(iVector2D), uv);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

iTexture** sliceTexture(int numX, int numY, const char* szFormat, ...)
{
	char szText[256];
	formating_text(szText, szFormat);
	const wchar_t* wStr = utf8_to_utf16(szText);
	Bitmap* bmp = Bitmap::FromFile(wStr, false);
	delete wStr;

	int w = bmp->GetWidth();
	int h = bmp->GetHeight();
	int x = w / numX;
	int y = h / numY;
	int potWidth = nextPot(x);
	int potHeight = nextPot(y);
	Rect rt(0,0,w,h);
	BitmapData bmpData;
	bmp->LockBits(&rt, ImageLockModeRead, PixelFormat32bppPARGB, &bmpData);

	iTexture** r = new iTexture*[numX * numY];
	int stride = bmpData.Stride/4;
	uint32* pixel = (uint32*)bmpData.Scan0;
	uint32* dst = new uint32[potHeight * potWidth];
	int idx = 0;

	for (int i = 0; i < numY; i++)
	{
		for (int j = 0; j < numX; j++)
		{
			memset(dst, 0, sizeof(uint32) * potHeight * potWidth);

			for (int k = 0; k < y; k++)
			{
				for (int o = 0; o < x; o++)
				{					
					uint8* d = (uint8*)&dst[potWidth * k + o];
					uint8* s = (uint8*)&pixel[(stride * i * y + j * x) + (stride * k + o)];
					
					d[0] = s[2];
					d[1] = s[1];
					d[2] = s[0];
					d[3] = s[3];
				}
			}

			GLuint i;
			glGenTextures(1, &i);
			glBindTexture(GL_TEXTURE_2D, i);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGBA, potWidth, potHeight, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, dst);
			glBindTexture(GL_TEXTURE_2D, 0);

			iTexture* tex = new iTexture();
			tex->imgID = i;
			tex->width = x;
			tex->height = y;
			tex->potWidth = potWidth;
			tex->potHeight = potHeight;
			r[idx] = tex;
			idx++;
		}
	}

	bmp->UnlockBits(&bmpData);
	delete bmp;
	delete dst;

	return r;
}

//TODO : not working when width and height are small than development size
iRect viewPort;
void getViewPort(int width ,int height)
{
#if 1
	float wr = width / DEV_WIDTH;
	float hr = height / DEV_HEIGHT;

	if (width > height)
	{
		viewPort.size.x = DEV_WIDTH * wr;
		viewPort.origin.x = (width - viewPort.size.x) / 2;
		viewPort.size.y = height;
		viewPort.origin.y = 0;
	}
	else if(width < height)
	{
		viewPort.size.x = width;
		viewPort.origin.x = 0;
		viewPort.size.y = DEV_HEIGHT * hr;
		viewPort.origin.y = (height - viewPort.size.y) / 2;
	}
#endif
}

iVector2D coordinate(iVector2D pos)
{
	float wr = DEV_WIDTH / viewPort.size.x;
	float hr = DEV_HEIGHT / viewPort.size.y;

	iVector2D r;
	r.x = (pos.x - viewPort.origin.x) * wr;
	r.y = (pos.y - viewPort.origin.y) * hr;

	return r;
}

char* readFile(int& len , const char* filePath, ...)
{
	char szText[256];
	formating_text(szText, filePath);

	FILE* f = fopen(szText, "rb");
	if (!f) return NULL;

	fseek(f, 0, SEEK_END);
	len = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* buf = new char[len + 1];
	fread(buf, sizeof(char), len, f);
	buf[len] = 0;
	fclose(f);

	return buf;
}

void saveFile(char* buf, int len, const char* filePath, ...)
{
	char szText[256];
	formating_text(szText, filePath);

	FILE* f = fopen(szText, "wb");
	if (!f)
	{
		std::ofstream o(szText);
		f = fopen(szText, "wb");
	}

	fwrite(buf, sizeof(char), len, f);
	fclose(f);
}



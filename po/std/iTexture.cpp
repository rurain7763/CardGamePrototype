#include"iTexture.h"
#include"iStd.h"
#include"iWindows.h"

iTexture::iTexture()
{
	imgID = -1;
	width, height, potWidth, potHeight = 0;
}

iTexture::~iTexture()
{
	glDeleteTextures(1, &imgID);
}

bool iTexture::createImage(const char* szFormat, ...)
{
	char szText[1024];
	formating_text(szText, szFormat);

	const wchar_t* fp = utf8_to_utf16(szText);
	Bitmap* bmp = Bitmap::FromFile(fp, false);
	delete fp;

	if (!bmp) return false;

	width = bmp->GetWidth();
	height = bmp->GetHeight();

	BitmapData bd;
	Rect rt(0, 0, width, height);
	bmp->LockBits(&rt, ImageLockModeRead , PixelFormat32bppARGB, &bd);

	potWidth = nextPot(width);
	potHeight = nextPot(height);

	int stride = bd.Stride / 4;
	uint32* src = (uint32*)bd.Scan0;
	uint32* dst = new uint32[potWidth * potHeight];
	memset(dst, 0, sizeof(uint32) * potWidth * potHeight);

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			uint8* s = (uint8*)&src[stride * j + i];
			uint8* d = (uint8*)&dst[potWidth * j + i];

			d[0] = s[2];
			d[1] = s[1];
			d[2] = s[0];
			d[3] = s[3];
		}
	}

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGBA, potWidth, potHeight, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, dst);
	glBindTexture(GL_TEXTURE_2D, 0);

	bmp->UnlockBits(&bd);
	delete bmp;
	delete dst;

	imgID = tex;
	
	return true;
}

bool iTexture::createImage(int w,int h)
{
	width = w;
	height = h;
	potWidth = nextPot(w);
	potHeight = nextPot(h);

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, potWidth, potHeight, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	imgID = tex;

	return true;
}

void iTexture::freeImage()
{
	if(imgID) 	glDeleteTextures(1, &imgID);
	imgID = 0;
	width, height ,potWidth	,potHeight = 0;
}






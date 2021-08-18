#include"iGraphics.h"
#include"iStd.h"

iBitmap::iBitmap()
{
    bmp = NULL;
    width = 0;
    height = 0;
}

iBitmap::~iBitmap()
{
    delete bmp;
}

iGraphics* iGraphics::s = nullptr;

iGraphics::iGraphics()
{
    _bmp = NULL;

    pr =1.f, pg=1.f, pb=1.f, pa=1.f;
    pw = 1.f;

	ba=1.f, br=0.f, bg=0.f, bb=0.f;
	brush = new SolidBrush(Color(ba*255,br*255,bg*255,bb*255));
	fontSize = 1;
	borderSize = 0;
}

iGraphics::~iGraphics()
{
	delete (Graphics*)graphicsForString;
	delete (Bitmap*)bmpForString;
	delete (SolidBrush*)brush;
    delete bB;
    delete bmp;
    delete fB;
    GdiplusShutdown(token);
}

void iGraphics::start(HDC hdc)
{
    GdiplusStartupInput in;
    GdiplusStartup(&token, &in, NULL);

    fB = new Graphics(hdc);
    fB->SetPageUnit(UnitPixel);
    //g->SetPageScale(1.0f);
    fB->SetPixelOffsetMode(PixelOffsetModeHalf);
    fB->SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
    fB->SetTextContrast(0xffffffff);
    fB->SetCompositingMode(CompositingModeSourceOver);
    fB->SetCompositingQuality(CompositingQualityAssumeLinear);
    fB->SetSmoothingMode(SmoothingModeAntiAlias8x8);
    fB->SetInterpolationMode(InterpolationModeHighQualityBicubic);

    bmp = new Bitmap(DEV_WIDTH, DEV_HEIGHT, PixelFormat32bppARGB);
    bB = Graphics::FromImage(bmp);

    bB->SetPageUnit(UnitPixel);
    //g->SetPageScale(1.0f);
    bB->SetPixelOffsetMode(PixelOffsetModeHalf);
    bB->SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
    bB->SetTextContrast(0xffffffff);
    bB->SetCompositingMode(CompositingModeSourceOver);
    bB->SetCompositingQuality(CompositingQualityAssumeLinear);
    bB->SetSmoothingMode(SmoothingModeAntiAlias8x8);
    bB->SetInterpolationMode(InterpolationModeHighQualityBicubic);

    bmpForString = new Bitmap(DEV_WIDTH, DEV_HEIGHT, PixelFormat32bppARGB);
    graphicsForString = (Graphics*)Graphics::FromImage((Bitmap*)bmpForString);
}

void iGraphics::init(int w, int h)
{
    if (_bmp)
    {
        delete _bmp;
        delete _bmpG;
    }

    _bmp = new Bitmap(w, h, PixelFormat32bppARGB);
    _bmpG = Graphics::FromImage(_bmp);

    _bmpG->SetPageUnit(UnitPixel);
    //_bmpG->SetPageScale(1.0f);
    _bmpG->SetPixelOffsetMode(PixelOffsetModeHalf);
    _bmpG->SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
    _bmpG->SetTextContrast(0xffffffff);
    _bmpG->SetCompositingMode(CompositingModeSourceOver);
    _bmpG->SetCompositingQuality(CompositingQualityAssumeLinear);
    _bmpG->SetSmoothingMode(SmoothingModeAntiAlias8x8);
    _bmpG->SetInterpolationMode(InterpolationModeHighQualityBicubic);

    _bmpG->Clear(Color(0, 0, 0, 0));
}

iTexture* iGraphics::getTexture()
{
    if (!_bmp) return NULL;
    int width = _bmp->GetWidth();
    int height = _bmp->GetHeight();

    iTexture* r = new iTexture();

    BitmapData bmpData;
    Rect rt(0,0,width,height);
    _bmp->LockBits(&rt, ImageLockModeRead, PixelFormat32bppARGB, &bmpData);
    
    int potWidth = nextPot(width);
    int potHeight = nextPot(height);

    int stride = bmpData.Stride / 4;
    uint32* src = (uint32*)bmpData.Scan0;
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, potWidth, potHeight, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, dst);
    glBindTexture(GL_TEXTURE_2D, 0);

    _bmp->UnlockBits(&bmpData);
    delete _bmp;
    _bmp = NULL;
    delete _bmpG;
    _bmpG = NULL;
    delete dst;

    r->imgID = tex;
    r->width = width;
    r->height = height;
    r->potWidth = potWidth;
    r->potHeight = potHeight;

    return r;
}

void iGraphics::setColor(float _r, float _g, float _b, float _a)
{
    pr = _r; pg = _g; pb = _b; pa = _a;
}

void iGraphics::drawLine(iVector2D s, iVector2D e)
{
    Pen pen(Color(pa * 255, pr * 255, pg * 255, pb * 255), pw);
    bB->DrawLine(&pen, s.x, s.y, e.x, e.y);
}

void iGraphics::drawLine(float sx, float sy, float ex, float ey)
{
    Pen pen(Color(pa * 255, pr * 255, pg * 255, pb * 255), pw);
    bB->DrawLine(&pen, sx, sy, ex, ey);
}

void iGraphics::drawRect(iRect rt)
{
    Pen pen(Color(pa * 255, pr * 255, pg * 255, pb * 255), pw);
    GraphicsPath path;

    float x = rt.origin.x, width = rt.size.x;
    float y = rt.origin.y, height = rt.size.y;

    path.AddLine(x, y, x + width, y);
    path.AddLine(x + width, y, x + width, y + height);
    path.AddLine(x + width, y + height, x, y + height);
    path.AddLine(x, y + height, x, y);
    path.CloseFigure();

    bB->DrawPath(&pen, &path);
}

void iGraphics::fillRect(iRect rt)
{
    SolidBrush brush(Color(pa * 255, pr * 255, pg * 255, pb * 255));
    GraphicsPath path;

    float x = rt.origin.x, width = rt.size.x;
    float y = rt.origin.y, height = rt.size.y;

    path.AddLine(x, y, x + width, y);
    path.AddLine(x + width, y, x + width, y + height);
    path.AddLine(x + width, y + height, x, y + height);
    path.AddLine(x, y + height, x, y);
    path.CloseFigure();

    _bmpG->FillPath(&brush, &path);
}

void iGraphics::fillRect(float top, float left, float right, float bottom)
{
    SolidBrush brush(Color(pa * 255, pr * 255, pg * 255, pb * 255));
    GraphicsPath path;

    float x = top, width = right - left;
    float y = left, height = bottom - top;

    path.AddLine(x, y, x + width, y);
    path.AddLine(x + width, y, x + width, y + height);
    path.AddLine(x + width, y + height, x, y + height);
    path.AddLine(x, y + height, x, y);
    path.CloseFigure();

    _bmpG->FillPath(&brush, &path);
}

iBitmap** iGraphics::sliceBitmap(int numX, int numY, const char* szFormat, ...)
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
    Rect rt(0, 0, w, h);
    BitmapData bmpData;
    bmp->LockBits(&rt, ImageLockModeRead, PixelFormat32bppPARGB, &bmpData);

    int numXY = numX * numY;
    iBitmap** r = new iBitmap * [numXY];
    int stride = bmpData.Stride / 4;
    uint32* pixel = (uint32*)bmpData.Scan0;

    for (int i = 0; i < numXY; i++)
    {
        int xp = i % numX;
        int yp = i / numX;

        Bitmap* b = new Bitmap(x,y,PixelFormat32bppARGB);
        Rect rect(0, 0, x, y);
        BitmapData bd;
        b->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppARGB, &bd);
        uint32* bp = (uint32*)bd.Scan0;
        int strd = bd.Stride / 4;

        for (int j = 0; j < y; j++)
        {
            memcpy(&bp[strd * j], &pixel[stride * (yp*y) + (xp*x) + (stride*j)], sizeof(uint32) * x);
        }
        b->UnlockBits(&bd);

        iBitmap* ib = new iBitmap();        
        ib->bmp = b;
        ib->width = x;
        ib->height = y;

        r[i] = ib;
    }
    
    bmp->UnlockBits(&bmpData);

    return r;
}

void iGraphics::drawImage(iBitmap* tex, iVector2D pos, int anc, 
    iVector2D iPos, iVector2D iSize, iVector2D size, 
    int rXYZ, float degree, int reverse)
{
    iVector2D _size = iVector2DMake(iSize.x * size.x, iSize.y * size.y);

    iVector2D dstPoint[3]
    {
        pos,
        {pos.x + _size.x , pos.y},
        {pos.x , pos.y + _size.y}
    };

    ColorMatrix mat =
    {
        1.f, 0.f, 0.f, 0.f, 0.f,
        .0f, 1.f, 0.f, 0.f, 0.f,
        .0f, 0.f, 1.f, 0.f, 0.f,
        .0f, 0.f, 0.f,  pa, 0.f,
        .0f, 0.f, 0.f, 0.f, 1.f,
    };

    ImageAttributes attr;
    attr.SetColorMatrix(&mat, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

    _bmpG->DrawImage((Image*)tex->bmp, (PointF*)dstPoint, 3,
        iPos.x, iPos.y, iSize.x, iSize.y, UnitPixel, &attr);
}

void iGraphics::setFontName(const char* sn)
{
    int len = strlen(sn);

    if (stringName)
        delete stringName;
    
    stringName = new char[len+1];
    memcpy(stringName, sn, sizeof(char) * len);
    stringName[len] = 0;   
}

void iGraphics::setStringColor(float r, float g, float b, float a)
{
    ba = a; br = r; bg = g; bb = b;
}

iGraphics* iGraphics::share()
{
	if (!s) s = new iGraphics();
	return s;
}

void iGraphics::checkFontFamily(void* fontFamily,int& fs)
{
	FontFamily* ff = (FontFamily*)fontFamily;

    char* path = stringName;

    if (path[0] != 'a' || path[1] != 's' || path[2] != 's')
    {
        wchar_t* wstr = utf8_to_utf16(path);
        FontFamily f(wstr);
        fs = FontStyleRegular;

        memcpy(ff, &f, sizeof(FontFamily));
        delete wstr;
    }
    else
    {
        PrivateFontCollection* pfc = new PrivateFontCollection();

        const wchar_t* wstr = utf8_to_utf16(stringName);
        pfc->AddFontFile(wstr);
        delete wstr;

        int count = pfc->GetFamilyCount();
        int found;
        pfc->GetFamilies(count, ff, &found);

        delete pfc;

        for (int i = 0; i < found; i++)
        {
            if (ff->IsStyleAvailable(FontStyleRegular))
            {
                fs = FontStyleRegular;
                return;
            }
            else if (ff->IsStyleAvailable(FontStyleBold))
            {
                fs = FontStyleBold;
                return;
            }
        }
    }
}

iRect iGraphics::stringRect(const char* szText)
{
    Bitmap* bmp = (Bitmap*)bmpForString;
    Graphics* g = (Graphics*)graphicsForString;

    g->Clear(Color(0, 0, 0, 0));

    GraphicsPath path;
    StringFormat sf;
    FontFamily ff;
    int fontStyle;

    checkFontFamily(&ff, fontStyle);
    wchar_t* wStr = utf8_to_utf16(szText);
    path.AddString(wStr, lstrlenW(wStr), &ff, fontStyle, fontSize, PointF(0, 0), &sf);
    delete wStr;

    SolidBrush b(Color(ba * 255, br * 255, bg * 255, bb * 255));
    g->FillPath(&b, &path);

    Rect rt(0, 0, DEV_WIDTH, DEV_HEIGHT);
    BitmapData bmpData;
    bmp->LockBits(&rt, ImageLockModeRead, PixelFormat32bppPARGB, &bmpData);

    uint8* rgba = (uint8*)bmpData.Scan0;

    int left = bmpData.Width , top = bmpData.Height, 
        right = -1, bottom = -1;
    int i, j = 0;

    for (i = 0; i < bmpData.Width; i++)
    {
        bool exit = false;

        for (j = 0; j < bmpData.Height; j++)
        {
            if (rgba[bmpData.Stride * j + (i * 4) + 3])
            {
                exit = true;
                break;
            }
        }
        if (exit)
        {
            left = i;
            break;
        }
    }

    for (i = bmpData.Width - 1; i > -1; i--)
    {
        bool exit = false;

        for (j = 0; j < bmpData.Height; j++)
        {
            if (rgba[bmpData.Stride * j + (i * 4) + 3])
            {
                exit = true;
                break;
            }
        }
        if (exit)
        {
            right = i;
            break;
        }
    }

    for (i = 0; i < bmpData.Height; i++)
    {
        bool exit = false;

        for (j = 0; j < bmpData.Width; j++)
        {
            if (rgba[bmpData.Stride * i + (j * 4) + 3])
            {
                exit = true;
                break;
            }
        }
        if (exit)
        {
            top = i;
            break;
        }
    }

    for (i = bmpData.Height - 1; i > -1; i--)
    {
        bool exit = false;

        for (j = 0; j < bmpData.Width; j++)
        {
            if (rgba[bmpData.Stride * i + (j * 4) + 3])
            {
                exit = true;
                break;
            }
        }
        if (exit)
        {
            bottom = i;
            break;
        }
    }

    bmp->UnlockBits(&bmpData);
    iVector2D origin = iVector2DMake(left,top);
    iVector2D size = iVector2DMake(right - left + 1, bottom - top + 1);

    return iRect(origin,size);
}

void iGraphics::drawString(iVector2D& pos, int anc, const char* szFormat, ...)
{
	char szText[1024];
	formating_text(szText, szFormat);
	
    iRect rt = stringRect(szText);
    int w = rt.size.x - rt.origin.x , h = rt.size.y - rt.origin.y;

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

    float x = pos.x - rt.origin.x, y = pos.y - rt.origin.y;

    GraphicsPath path;
    StringFormat sf;
    FontFamily ff;
    int fontStyle;
    checkFontFamily(&ff, fontStyle);

    const wchar_t* wStr = utf8_to_utf16(szText);
    path.AddString(wStr, lstrlenW(wStr), &ff, fontStyle, fontSize,
                   PointF(x,y), &sf);
    delete wStr;

    SolidBrush b(Color(ba * 255, br * 255, bg * 255, bb * 255));
	_bmpG->FillPath(&b,&path);
}

void iGraphics::drawString(void* g,iVector2D& pos, int anc, const char* szFormat, ...)
{
    char szText[1024];
    formating_text(szText, szFormat);

    iRect rt = stringRect(szText);
    float x = pos.x - rt.origin.x, y = pos.y - rt.origin.y;

    GraphicsPath path;
    StringFormat sf;
    FontFamily ff;
    int fontStyle;
    checkFontFamily(&ff, fontStyle);

    const wchar_t* wStr = utf8_to_utf16(szText);
    path.AddString(wStr, lstrlenW(wStr), &ff, fontStyle, fontSize,
                   PointF(x, y), &sf);
    delete wStr;

    SolidBrush b(Color(ba * 255, br * 255, bg * 255, bb * 255));
    ((Graphics*)g)->FillPath(&b, &path);
}

iTexture* iGraphics::getStringTexInRect(int w,int h, int inter, const char* szFormat, ...)
{
    char szText[255];
    formating_text(szText, szFormat);

    int potWidth = nextPot(w);
    int potHeight = nextPot(h);

    Bitmap* b = new Bitmap(potWidth, potHeight, PixelFormat32bppARGB);
    Graphics* bg = Graphics::FromImage(b);

    int width = b->GetWidth();
    int height = b->GetHeight();
    
    int len = strlen(szText);
    iVector2D pos = iVector2DMakeZero;
    char copy[255];
    int off = 0;
    int lineNum = 0;
    int sw = 0;
    int sh = 0;

    for (int i = 0; i < len; i++)
    {
        int copyLen = (i + 1) - off;
        memcpy(copy, &szText[off], sizeof(char)* copyLen);
        copy[copyLen] = 0;

        iRect strRt = stringRect(copy);
        sw = strRt.size.x - strRt.origin.x; 
        int _sh = strRt.size.y - strRt.origin.y;
        if(_sh > sh) sh = _sh;

        if (sw > w || szText[i] == '\n')
        {
            if(lineNum == 0) pos.y = (sh * lineNum);
            else  pos.y = (sh * lineNum) + inter;

            copy[copyLen - 1] = 0;
            drawString(bg, pos, BOTTOM | RIGHT, copy);
            off = i;
            lineNum++;
            i--;
        }
    }

    pos.y = (sh * (lineNum+1)) + inter;
    if (lineNum == 0) pos.y = 0;
    drawString(bg, pos, BOTTOM | RIGHT, copy);

    BitmapData bmpData;
    Rect texRt(0, 0, w, h);
    b->LockBits(&texRt, ImageLockModeRead, PixelFormat32bppARGB, &bmpData);

    int stride = bmpData.Stride / 4;
    uint32* src = (uint32*)bmpData.Scan0;
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, potWidth, potHeight, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, dst);
    glBindTexture(GL_TEXTURE_2D, 0);

    b->UnlockBits(&bmpData);
    delete b;
    delete[] dst;

    iTexture* r = new iTexture();
    r->imgID = tex;
    r->width = width;
    r->height = height;
    r->potWidth = potWidth;
    r->potHeight = potHeight;

    return r;
}

iBitmap* iGraphics::getStringBmpInRect(int w, int h, int inter, const char* szFormat, ...)
{
    char szText[255];
    formating_text(szText, szFormat);

    int potWidth = nextPot(w);
    int potHeight = nextPot(h);

    Bitmap* b = new Bitmap(potWidth, potHeight, PixelFormat32bppARGB);
    Graphics* bg = Graphics::FromImage(b);

    int width = b->GetWidth();
    int height = b->GetHeight();
    
    int len = strlen(szText);
    iVector2D pos = iVector2DMakeZero;
    char copy[255];
    int off = 0;
    int lineNum = 0;
    int sw = 0;
    iRect _strRt = stringRect(szText);
    int sh = (_strRt.size.y - _strRt.origin.y) + inter;

    for (int i = 0; i < len; i++)
    {
        int copyLen = (i + 1) - off;
        memcpy(copy, &szText[off], sizeof(char)* copyLen);
        copy[copyLen] = 0;

        iRect strRt = stringRect(copy);
        sw = strRt.size.x - strRt.origin.x; 

        if (sw > w || szText[i] == '\n')
        {
            pos.y = sh * lineNum;

            copy[copyLen - 1] = 0;
            drawString(bg, pos, BOTTOM | RIGHT, copy);
            off = i;
            lineNum++;
            i--;
        }
    }

    pos.y = sh * lineNum;
    if (strlen(copy) == 1 && copy[0] == '.') pos.y += sh;
    drawString(bg, pos, BOTTOM | RIGHT, copy);

    iBitmap* r = new iBitmap();
    r->bmp = b;
    r->width = width;
    r->height = height;

    return r;
}



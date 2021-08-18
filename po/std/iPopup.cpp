#include"iPopup.h"
#include"iStd.h"

//----------------------------------------- iPopupObj
iPopupObject::iPopupObject()
{
	bShow = true;
	active = false;
	idx = -1;
	r = 1, g = 1, b = 1, a = 1;
	br = 1, bg = 1, bb = 1, ba = 1;
	relatePos = iVector2DMakeZero;
	relateSize = iVector2DMakeZero;
}

iPopupObject::~iPopupObject()
{
}

void iPopupObject::initColor(float r, float g, float b, float a)
{
	this->r = r; this->g = g; this->b = b; this->a = a;
	br = r; bg = g; bb = b; ba = a;
}

void iPopupObject::setColor(float rr, float gg, float bb, float aa)
{
	br = r; bg = g; bb = b; ba = a;
	r = rr; g = gg; b = bb; a = aa;
}

//----------------------------------------- iPopupBotton
Botton::Botton()
{
	curStr = "";
	str = "";
	strTex = NULL;
	onClick = NULL;
	onMouseOverlap = NULL;
	tex = NULL;
}

Botton::~Botton()
{
	if(strTex) delete strTex;
}

void Botton::paint(float dt,iVector2D off)
{
	iInputManagement* im = iInputManagement::share();

	iRect rt = iRectMake(off.x + relatePos.x, off.y + relatePos.y,
						 relateSize.x, relateSize.y);

	if (active)
	{
		if (onMouseOverlap && containPoint(im->mp, rt))
		{
			onMouseOverlap(this);

			if (onClick && im->mso & MOUSE_LDOWN)
			{
				onClick(this);
			}
		}
	}

	if (tex)
	{
		drawImage(tex, rt.origin, BOTTOM | RIGHT,
				  { 0,0 }, { (float)tex->width,(float)tex->height },
				  { 1,1 }, 2, 0);
	}
	else
	{
		setRGBA(r, g, b, a);
		fillRect(rt);
		setRGBA(1, 1, 1, 1);
	}

	iVector2D pos = iVector2DMakeZero;
	iGraphics* ig = iGraphics::share();

	if (strcmp(curStr.str, str.str))
	{
		ig->fontSize=30;
		ig->setFontName("Arial");
		ig->setStringColor(0, 0, 0, 1);
		ig->init(rt.size.x,rt.size.y);
		ig->drawString(ig->_bmpG,pos, BOTTOM | RIGHT, str.str);
		ig->setStringColor(1, 1, 1, 1);
		strTex = ig->getTexture();
		curStr = str;
		iRect r = ig->stringRect(str.str);
		strW = r.size.x; strH = r.size.y;
	}
	
	if (strTex)
	{
		pos.x = rt.origin.x + rt.size.x / 2;
		pos.y = rt.origin.y + rt.size.y / 2;

		setRGBA(1, 1, 1, 1);
		drawImage(strTex, pos, VCENTER | HCENTER,
			{ 0,0 }, { (float)strW,(float)strH }, { 1,1 },
			2, 0 ,REVERSE_NONE);
		setRGBA(1, 1, 1, 1);
	}

	if (br != 0 || bg != 0 || bb != 0 || ba != 0)
	{
		r = br; g = bg; b = bb; a = ba;
	}
}

//----------------------------------------- iPopupImage
iPopupImage::iPopupImage()
{
	tex = nullptr;
}

iPopupImage::~iPopupImage()
{
	delete tex;
}

void iPopupImage::paint(float dt, iVector2D off)
{
	setRGBA(r, g, b, a);
	float w = (float)tex->width; float h = (float)tex->height;
	drawImage(tex, off + relatePos , BOTTOM | RIGHT,
			 { 0,0 }, { w,h },
			 { relateSize.x / w , relateSize.y /h}, 2, 0);
	setRGBA(1, 1, 1, 1);

	if (br != 0 || bg != 0 || bb != 0 || ba != 0)
	{
		r = br; g = bg; b = bb; a = ba;
	}
}

//----------------------------------------- iPopupTextBox
iPopupTextBox::iPopupTextBox()
{
	curStr = "";
	str = "";
	strTex = NULL;
	backR = 0, backG = 0, backB = 0, backA = 0;
	fontSize = 40;
}

iPopupTextBox::~iPopupTextBox()
{
	delete strTex;
}

void iPopupTextBox::setText(const char* szFormat,...)
{
	char szText[1024];
	formating_text(szText, szFormat);

	if (!strcmp(str.str, szText)) return;

	if (strTex) delete strTex;
	iGraphics* ig = iGraphics::share();
	iVector2D pos = iVector2DMakeZero;

	ig->init(relateSize.x, relateSize.y);
	ig->fontSize = fontSize;
	ig->setFontName("Arial");
	ig->setStringColor(r, g, b, a);
	ig->drawString(pos, BOTTOM | RIGHT, szText);
	ig->setStringColor(1, 1, 1, 1);
	iRect rt = ig->stringRect(szText);
	strTex = ig->getTexture();

	curStr = szText;
	strW = rt.size.x; strH = rt.size.y;
}

void iPopupTextBox::setText(const iString& str)
{
	if (strTex) delete strTex;

	iGraphics* ig = iGraphics::share();
	iVector2D pos = iVector2DMakeZero;

	ig->fontSize = fontSize;
	ig->setFontName("Arial");
	ig->setStringColor(r, g, b, a);
	ig->init(relateSize.x, relateSize.y);
	ig->drawString(pos, BOTTOM | RIGHT, str.str);
	iRect r = ig->stringRect(str.str);
	strTex = ig->getTexture();

	curStr = str;
	strW = r.size.x; strH = r.size.y;
}

void iPopupTextBox::paint(float dt, iVector2D off)
{
	iVector2D pos = iVector2DMakeZero;
	iGraphics* ig = iGraphics::share();

	setRGBA(backR, backG, backB, backA);
	fillRect(iRect(relatePos, relateSize));
	setRGBA(1, 1, 1, 1);

	if (strTex)
	{
		pos.x = off.x + relatePos.x + relateSize.x / 2;
		pos.y = off.y + relatePos.y + relateSize.y / 2;
		
		setRGBA(1, 1, 1, 1);
		drawImage(strTex, pos, VCENTER | HCENTER,
				  { 0,0 }, { (float)strW,(float)strH }, 
				  { 1,1 },
				  2, 0);
		setRGBA(1, 1, 1, 1);
	}

	if (br != 0 || bg != 0 || bb != 0 || ba != 0)
	{
		r = br; g = bg; b = bb; a = ba;
	}
}

//----------------------------------------- iPopupFillBar
iPopupFillBar::iPopupFillBar()
{
	rate = 0;
	barR = 0, barG = 0, barB = 0, barA = 0;
}

iPopupFillBar::~iPopupFillBar()
{

}

void iPopupFillBar::paint(float dt, iVector2D off)
{
	if (!bShow) return;

	setRGBA(r, g, b, a);
	fillRect(iRect(relatePos, relateSize));
	setRGBA(1, 1, 1, 1);

	iVector2D s = iVector2DMakeZero;
	s.x = relateSize.x* clamp(rate, 0, 1);
	s.y = relateSize.y;

	setRGBA(barR, barG, barB, barA);
	fillRect(iRect(relatePos, s));
	setRGBA(1, 1, 1, 1);

	if (br != 0 || bg != 0 || bb != 0 || ba != 0)
	{
		r = br; g = bg; b = bb; a = ba;
	}
}

//----------------------------------------- iPopupSlideImage
iPopupSlideImage::iPopupSlideImage()
{
	tex = new iArray();
	slideTex = NULL;

	off = iVector2DMakeZero;
	minPos =
	maxPos = iVector2DMakeZero;
	prevP = iVector2DMakeZero;

	prevTime = iFPS::share()->timeNow;
	force = 0.f;
	touching = false;

	style = iPopupSlideImageStyleHorizontal;
	curIdx = 0;
}

iPopupSlideImage::~iPopupSlideImage()
{
	delete tex;
	delete slideTex;
}

void iPopupSlideImage::addTex(iTexture* t ,int n)
{
	iVector2D maxSize = relateSize * n;

	int maxW = -1; int maxH = -1;
	for (int i = 0; i < n; i++)
	{
		iTexture* c = &t[i];
		tex->push_back(c);
		maxW=max(maxW, c->width);
		maxH=max(maxH, c->height);
	}

	minPos = iVector2DMake(0, 0);
	maxPos = iVector2DMake(maxSize.x - relateSize.x, maxSize.y - relateSize.y);

	float sx = relateSize.x / maxW;
	float sy = relateSize.y / maxH;

	if (style == iPopupSlideImageStyleVertical)
	{

	}
	else if (style == iPopupSlideImageStyleHorizontal)
	{
		slideTex = new iTexture();
		slideTex->createImage(maxSize.x, relateSize.y);
		fbo->bind(slideTex);
		for (int i = 0; i < n; i++)
		{
			iTexture* c = &t[i];
			iVector2D pos = iVector2DMake(relateSize.x * i, 0);
			setRGBA(1, 1, 1, 1);
			drawImage(c, pos, BOTTOM | RIGHT,
				{ 0,0 },
				{ (float)c->width,(float)c->height },
				{ sx,sy },
				2, 0);
			setRGBA(1, 1, 1, 1);
		}
		fbo->unbind();
	}
}

void iPopupSlideImage::paint(float dt, iVector2D _off)
{
	//draw
	setRGBA(1, 1, 1, 1);
	drawImage(	slideTex, _off + relatePos , VCENTER | HCENTER,
				off,
				relateSize,
				{ 1,1 },
				2, 0, REVERSE_HEIGHT);
	setRGBA(1, 1, 1, 1);

	iInputManagement* ip = iInputManagement::share();

	iFPS* fps = iFPS::share();
	if (ip->mso & MOUSE_LDOWN)
	{
		force = 0.f;
		prevP = ip->mp;
		prevTime = fps->timeNow;
		touching = true;
	}
	else if (ip->ms == (MOUSE_MOVE | MOUSE_LDOWN))
	{
		iVector2D move = ip->mp - prevP;
		float len = move.length();
		float delta = (fps->timeNow - prevTime) * .001f;

		if (delta != 0 && len > 3.f)
		{
			dir = move / len;
			force = len / delta;
			if (force >= 2100.f) force = 2100.f;

			prevP = ip->mp;
			prevTime = fps->timeNow;
		}
	}
	else if (ip->mso & MOUSE_LUP)
	{
		touching = false;
	}

	if (force)
	{
		force -= 700 * dt;

		if (iPopupSlideImageStyleHorizontal)
		{
			off.x += dir.x * (force * dt);
			if (off.x < minPos.x) off.x = minPos.x;
			else if (off.x > maxPos.x) off.x = maxPos.x;
			curIdx = (off.x + relateSize.x * .5f) / relateSize.x;
		}
		else //if (iPopupSlideImageStyleVertical)
		{
			off.y += dir.y * (force * dt);
			if (off.y < minPos.y) off.y = minPos.y;
			else if (off.y > maxPos.y) off.y = maxPos.y;
			curIdx = (off.y + relateSize.y * .5f) / relateSize.y;
		}

		if (force < 0.f) force = 0;
	}
	else if(!touching)
	{
		if (iPopupSlideImageStyleHorizontal)
		{
			float t = curIdx * relateSize.x;

			if (off.x < t)
			{
				float dist = (t - off.x);
				off.x +=  4.f * dist * dt;
				if (off.x > t) off.x = t;
			}
			else if (off.x > t)
			{
				float dist = (off.x - t);
				off.x -= 4.f * dist * dt;
				if (off.x < t) off.x = t;
			}
		}
		else //if (iPopupSlideImageStyleVertical)
		{
			float t = curIdx * relateSize.y;

			if (off.y < t)
			{
				float dist = (t - off.y);
				off.y += 4.f * dist * dt;
				if (off.y > t) off.y = t;
			}
			else if (off.y > t)
			{
				float dist = (off.y - t);
				off.y -= 4.f * dist * dt;
				if (off.y < t) off.y = t;
			}
		}	
	}
}

//-------------------------------iPopupScrollBar
iPopupScrollBar::iPopupScrollBar(float x, float y, int w, int h) 
	: iPopupObject()
{
	iGraphics* g = iGraphics::share();
	iVector2D size = iVector2DMake(w, h);
	g->init(size.x, size.y);
	g->setColor(1, 1, 1, 1);
	g->fillRect(0, 0, w, h);
	g->setColor(1, 1, 1, 1);
	boxTex = g->getTexture();

	relatePos = iVector2DMake(x, y);
	relateSize = size;

	rate = 0.f;
	maxShowSize = iVector2DMake(1,1);
	contentsSize = iVector2DMake(1,1);
}

iPopupScrollBar::~iPopupScrollBar()
{
	delete boxTex;
}

void iPopupScrollBar::paint(float dt, iVector2D off)
{
	iVector2D boxPos = off + relatePos;

	drawImage( boxTex, boxPos , BOTTOM | RIGHT,
			  { 0,0 }, { (float)boxTex->width,(float)boxTex->height },
			  { 1,1 }, 2, 0);

	float barSize = relateSize.y * (maxShowSize.y / contentsSize.y);
	float barPosY = boxPos.y + (relateSize.y - barSize) * rate;

	iRect bar = iRectMake(off.x + relatePos.x, off.y + barPosY, relateSize.x, barSize);
	setRGBA(1, 0, 0, 1);
	fillRect(bar);
	setRGBA(1, 1, 1, 1);
}

//-------------------------------iPopupScrollImage
iPopupScrollImage::iPopupScrollImage(iVector2D pos, iVector2D ms, iVector2D cs)
	:iPopupObject()
{	
	objArr = new iArray();
	texForFBO = new iTexture();
	texForFBO->createImage(cs.x, cs.y);

	float barW = ms.x * .1f;
	if (barW >= 30) barW = 30.f;
	float barH = ms.y * .7f;
	iVector2D barPos = iVector2DMake(ms.x - (20 + barW), (ms.y - barH) * .5f);
	scrollBar = new iPopupScrollBar(pos.x + barPos.x, pos.y + barPos.y , barW, barH);
	scrollBar->maxShowSize = ms;
	scrollBar->contentsSize = cs;

	iGraphics* g = iGraphics::share();
	g->init(cs.x, cs.y);
	g->setColor(1, 1, 1, .5f);
	g->fillRect(iRectMake(0,0,cs.x,cs.y));
	g->setColor(1, 1, 1, 1);
	tex = g->getTexture();

	off = iVector2DMakeZero;
	minPos = off;
	maxPos = iVector2DMake((cs.x - ms.x),(cs.y - ms.y));

	relatePos = pos;
	relateSize = ms;
	force = 0;
	prevTime = 0;
	prevP = iVector2DMakeZero;
	dir = iVector2DMakeZero;

	rate = 0.f;
}

iPopupScrollImage::~iPopupScrollImage()
{
	delete scrollBar;
	delete tex;
	delete objArr;
	delete texForFBO;
}

void iPopupScrollImage::addObject(iPopupObject* obj)
{
	objArr->push_back(obj);
}

void iPopupScrollImage::paint(float dt, iVector2D _off)
{
	iVector2D pos = _off + relatePos;

	drawImage(tex, pos , BOTTOM | RIGHT,
			  off, relateSize,
			  { 1,1 },
			  2, 0);

	fbo->bind(texForFBO);
	for (int i = 0; i < objArr->num; i++)
	{
		iPopupObject* obj = (iPopupObject*)objArr->at(i);
		if (before) before(obj);
		obj->paint(dt, pos);
		if (after) after(obj);
	}
	fbo->unbind();

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	drawImage(texForFBO, pos, BOTTOM | RIGHT,
			  off, 
			  { (float)texForFBO->width,(float)texForFBO->height },
			  { 1,1 }, 2, 0);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
						GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	scrollBar->paint(dt, pos);
	scrollBar->rate = rate;

	//key
	iInputManagement* ip = iInputManagement::share();
	iFPS* fps = iFPS::share();

	if (ip->mso & MOUSE_LDOWN)
	{
		force = 0.f;
		prevTime = fps->timeNow;
		prevP = ip->mp;
	}
	else if (ip->ms == (MOUSE_MOVE | MOUSE_LDOWN))
	{
		iVector2D move = ip->mp - prevP;
		float len = move.length();
		float delta = (fps->timeNow - prevTime) * .001f;

		if (delta != 0 && len > 3.f)
		{
			dir = move / len;
			force = len / delta;
			if (force >= 2100.f) force = 2100.f;

			prevP = ip->mp;
			prevTime = fps->timeNow;
		}
	}

	if (force)
	{
		force -= 700 * dt;

		off.y += dir.y * (force * dt);
		if (off.y < minPos.y) off.y = minPos.y;
		else if (off.y > maxPos.y) off.y = maxPos.y;

		if (force < 0.f) force = 0;
	}

	rate = off.y / maxPos.y;
}

//----------------------------------------- iPopup
iPopup::iPopup()
{
	objects = new iArray();
	bShow = false;
	stat = iPopupStatClose;
	style = iPopupStyleNone;
	sp = iVector2DMakeZero, ep = iVector2DMakeZero;
	delta = 0 , _delta = 5.f;
	texForPop = new iTexture();
	texForPop->createImage(DEV_WIDTH, DEV_HEIGHT);
	event = NULL;
}

iPopup::~iPopup()
{
	delete texForPop;
	delete objects;
}

void iPopup::addObject(iPopupObject* obj)
{
	objects->push_back(obj);
}

void iPopup::paint(float dt)
{
	if (!bShow) return;

	float alpha =1.f;
	iVector2D size = iVector2DMake(1,1);
	iVector2D pos = ep;

	if (style == iPopupStyleAlpha)
	{
		if (stat == iPopupStatOpen)
		{
			delta += dt;
			alpha = delta / _delta;

			if (alpha >= 1.f)
			{
				stat = iPopupStatProc;
				delta = 0;
				if (event) event(stat);
			}
		}
		else if (stat == iPopupStatProc)
		{
			alpha = 1;
			if (event) event(stat);
		}
		else if (stat == iPopupStatClose)
		{
			delta += dt;
			alpha = 1 - delta / _delta;

			if (alpha >= 1.f)
			{
				stat = iPopupStatClose;
				bShow = false;
				delta = 0;
				if (event) event(stat);
			}
		}
	}
	else if (style == iPopupStyleMove)
	{
		if (stat == iPopupStatOpen)
		{
			delta += dt;
			float r = delta / _delta;
			pos = linear(sp, ep,r);

			if (r >= 1.f)
			{
				stat = iPopupStatProc;
				delta = 0;
				if (event) event(stat);
			}
		}
		else if (stat == iPopupStatProc)
		{
			pos = ep;
			if (event) event(stat);
		}
		else if (stat == iPopupStatClose)
		{
			delta += dt;
			float r = delta / _delta;
			pos = linear(ep,sp,r);

			if (r >= 1.f)
			{
				bShow = false;
				delta = 0;
				if (event) event(stat);
			}
		}
	}
	else if (style == iPopupStyleZoom)
	{
		
	}
	else if (style == iPopupStyleNone)
	{
		if (stat == iPopupStatOpen)
		{
			stat = iPopupStatProc;
			if (event) event(stat);
		}
		else if (stat == iPopupStatProc)
		{
			if (event) event(stat);
		}
		else if (stat == iPopupStatClose)
		{
			bShow = false;
			delta = 0;
			if (event) event(stat);	
		}
	}

	fbo->bind(texForPop);
	fbo->clear(0, 0, 0, 0);
	for (int i = 0; i < objects->num; i++)
	{
		iPopupObject* po = (iPopupObject*)objects->at(i);
		po->a = alpha; 
		po->active = (stat == iPopupStatProc);
		iVector2D off = pos;
		off.x -= texForPop->width / 2;
		off.y -= texForPop->height / 2;
		po->paint(dt,off);
	}
	fbo->unbind();

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	drawImage(	texForPop, pos, VCENTER | HCENTER, iVector2DMake(0, 0),
				iVector2DMake(texForPop->width, texForPop->height), iVector2DMake(1,1),
				2, 0 , REVERSE_HEIGHT);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
						GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void iPopup::show(bool s)
{
	if (s)
	{
		if (stat != iPopupStatClose) return;

		bShow = s;
		stat = iPopupStatOpen;
		delta = 0;
	}
	else //if(!s)
	{
		if (stat != iPopupStatProc) return;

		stat = iPopupStatClose;
		delta = 0;
	}
}



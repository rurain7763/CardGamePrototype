#pragma once
#include"iType.h"
class iTexture;

class iFBO
{
public:
	iFBO(int w,int h);
	virtual ~iFBO();

	void clear(float r,float g,float b,float a);
	void bind();
	void bind(iTexture* tex);
	void unbind();

public:
	uint32 renderBuf;
	iTexture* colorBuf;
	uint32 frameBuf;

	iTexture* texStack[10];
	int stackNum;
};
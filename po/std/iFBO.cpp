#include"iFBO.h"
#include"iStd.h"

iFBO::iFBO(int w, int h)
{
	//render buffer or depth buffer
	glGenRenderbuffers(1, &renderBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuf);
	glRenderbufferStorage(	GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
							w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//color buffer(Texture)
	colorBuf = new iTexture();
	colorBuf->createImage(w, h);
	GLuint texID = colorBuf->imgID;

	//fbo
	glGenFramebuffers(1, &frameBuf);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
	glFramebufferTexture2D(	GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_2D, texID, 0);
	glFramebufferRenderbuffer(	GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
								GL_RENDERBUFFER, renderBuf);

	//check
	GLenum stat = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (stat != GL_FRAMEBUFFER_COMPLETE)
		printf("glCheckFramebufferStatus Error : %d\n", stat);
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	memset(texStack, 0, sizeof(texStack));
	stackNum = 0;
}

iFBO::~iFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteRenderbuffers(1, &renderBuf);
	delete colorBuf;
	glDeleteFramebuffers(1, &frameBuf);
}

void iFBO::clear(float r,float g,float b,float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void iFBO::bind()
{
	bind(colorBuf);
}

void iFBO::bind(iTexture* t)
{
	GLuint cb = t->imgID;
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
	glFramebufferTexture2D(	GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_2D, cb, 0);

	glViewport(0, 0, t->width, t->height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, t->width, t->height, 0, 0, 1);

	texStack[stackNum] = t;
	stackNum++;
}

void iFBO::unbind()
{
	stackNum--;
	if (stackNum)
	{
		stackNum--;
		bind(texStack[stackNum]);
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(	viewPort.origin.x,viewPort.origin.y,
				viewPort.size.x, viewPort.size.y);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, DEV_WIDTH, DEV_HEIGHT, 0, 0, 1);
}


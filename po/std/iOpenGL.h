#pragma once

#define GLEW_STATIC

#ifndef GLEW_STATIC
#error not define glew_static
#endif // !GLEW_STATIC

#include"iGraphics.h"
#include <gl/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/wglew.h>
#pragma comment(lib,"OpenGL32.lib")

class iFBO;
extern iFBO* fbo;

void loadOpenGL(HDC hdc);
void freeOpenGL();
void readyOpenGL();

#if 1 //=============================shader
#define VERTEX_SHADER 0 
#define FRAGMENT_SHADER 1
//compile
void checkShaderID(GLuint id);
void freeShaderID(GLuint id);
GLuint createShaderID(const char* filePath , GLuint flag);

//Link
void checkProgramID(GLuint id);
void freeProgramID(GLuint id);
GLuint createProgramID(GLuint vertexID, GLuint fragmentID);
#endif
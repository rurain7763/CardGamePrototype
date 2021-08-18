#include"iOpenGL.h"
#include"iStd.h"

iFBO* fbo;
HGLRC hrc;

void loadOpenGL(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;

	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;

	int pixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pixelFormat, &pfd);

	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	Log("Status : GL: %s , GLEW: %s\n", glGetString(GL_VERSION), glewGetString(GLEW_VERSION));
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		Log("Glew Error: %s\n", glewGetErrorString(err));
		return;
	}

#if 0 // openGL version setting
	if (wglewIsSupported("WGL_ARB_create_context"))
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hrc);

		int attr[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB,3,
			WGL_CONTEXT_MINOR_VERSION_ARB,2,
			WGL_CONTEXT_FLAGS_ARB,0,
			0
		};

		hrc = wglCreateContextAttribsARB(hdc, NULL, attr);
		wglMakeCurrent(hdc, hrc);

		Log("Status : GL: %s , GLEW: %s , SHADER: %s\n", glGetString(GL_VERSION), glewGetString(GLEW_VERSION)
			,glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
#endif

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

#if 0
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#else
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND_SRC);
	glBlendFuncSeparate(
		GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
		GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#endif
	glEnable(GL_BLEND);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glShadeModel(GL_SMOOTH);
	glFrontFace(GL_CCW); // gl_cw
	glDisable(GL_CULL_FACE);

	fbo = new iFBO(DEV_WIDTH, DEV_HEIGHT);
}

void freeOpenGL()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hrc);
	delete fbo;
}

void readyOpenGL()
{
	glViewport(	viewPort.origin.x, viewPort.origin.y,
				viewPort.size.x, viewPort.size.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, DEV_WIDTH, DEV_HEIGHT, 0, 0, 1);
}

//=============================================================shader
//compile
struct iShaderManager
{
	struct iProgramInfo
	{
		GLuint programID;
		GLuint* shaderID;
		int shaderIDNum;
	};

	iProgramInfo** programInfo;
	int piNum;
	int piSize;

	iShaderManager()
	{
		programInfo = new iProgramInfo * [5];
		piSize = 5;
		piNum = 0;
	}

	~iShaderManager()
	{
		for (int i = 0; i < piNum; i++)
			delete programInfo[i]->shaderID;
		delete programInfo;
	}

	void createProgram(const char* paths)
	{
		iString s = paths;
		int num;
		char** line = s.getStringLine(num);

		iProgramInfo* pi = new iProgramInfo();
		pi->programID = glCreateProgram();
		pi->shaderID = new GLuint[num];
		pi->shaderIDNum = 0;

		for (int i = 0; i < num; i++)
		{
			iString str = line[i];
			int fs, fe, vs, ve;
			str.find(".frag", fs, fe);
			str.find(".vert", vs, ve);

			if (fs != -1)
			{
				GLuint frag = createShaderID(line[i], FRAGMENT_SHADER);
				glAttachShader(pi->programID, frag);
				pi->shaderID[pi->shaderIDNum] = frag;
				pi->shaderIDNum++;
			}
			else if (vs != -1)
			{
				GLuint vert = createShaderID(line[i], VERTEX_SHADER);
				glAttachShader(pi->programID, vert);
				pi->shaderID[pi->shaderIDNum] = vert;
				pi->shaderIDNum++;
			}
		}

		glLinkProgram(pi->programID);

		for (int i = 0; i < num; i++)
		{
			GLuint shader = pi->shaderID[i];
			glDetachShader(pi->programID, shader);
		}

		if (pi->programID >= piSize)
		{
			iProgramInfo** copy = new iProgramInfo * [piSize + 5];
			memcpy(copy, programInfo, sizeof(iProgramInfo*) * piSize);
			delete programInfo;
			programInfo = copy;
			piSize += 5;
		}

		programInfo[pi->programID] = pi;
		piNum++;

		s.freeStringLine(line, num);
	}
};

void checkShaderID(GLuint id)
{
	GLint result = GL_FALSE;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result != GL_TRUE)
	{
		int len = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);

		char* str = new char[len + 1];
		glGetShaderInfoLog(id, len, NULL, str);
		showMessageBox("chekShaderID error", str);
		delete str;
	}
}

void freeShaderID(GLuint id)
{
	glDeleteShader(id);
}

GLuint createShaderID(const char* path, GLuint flag)
{
	int len;
	char* str = readFile(len, path);

	GLenum f[] = { GL_VERTEX_SHADER,	GL_FRAGMENT_SHADER };
	GLuint id = glCreateShader(f[flag]);
	glShaderSource(id, 1, &str, NULL);
	glCompileShader(id);

	checkShaderID(id);
	delete str;

	return id;
}

//Link
void checkProgramID(GLuint id)
{
	GLint result = GL_FALSE;
	glGetProgramiv(id, GL_LINK_STATUS, &result);

	if (result != GL_TRUE)
	{
		int len = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);

		char* str = new char[len + 1];
		glGetProgramInfoLog(id, len, NULL, str);
		showMessageBox("checkProgramID error", str);
		delete str;
	}
}

void freeProgramID(GLuint id)
{
	glDeleteProgram(id);
}

GLuint createProgramID(GLuint vs, GLuint fs)
{
	GLuint id = glCreateProgram();
	glAttachShader(id, vs);
	glAttachShader(id, fs);
	glLinkProgram(id);
	glDetachShader(id, vs);
	glDetachShader(id, fs);

	checkProgramID(id);

	return id;
}
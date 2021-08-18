#pragma once

//#define WIN32_LEAN_AND_MEAN             
#define GDIPVER 0x0110

#include <windows.h>
#include <CommCtrl.h>
#pragma comment(lib,"imm32.lib")
#pragma comment(lib,"comctl32.lib")

#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")
using namespace Gdiplus;

#include <tchar.h>
#include "../resource.h"

#include"iStd.h"

#ifdef _DEBUG
#pragma comment(linker ,"/entry:wWinMainCRTStartup /subsystem:console")
#define Log printf
#else
#define Log (void*)
#endif

extern Graphics* bB;

wchar_t* utf8_to_utf16(const char* szFormat, ...);
char* utf16_to_utf8(const wchar_t* wStr);

void start(HDC hdc,SCENE_METHOD m);
void drawApp(SCENE_UPDATE_METHOD m);
void end(SCENE_METHOD m);

//=============================================== win contents
void showMessageBox(const char* title, const char* message);

void initWndCtrlSystem();

HWND createWndStatic(HWND& parent,const char* str,
					 int x, int y, int width, int height);

HWND createWndButton(HWND& parent, const char* str, 
					 int x, int y, int width, int height);

HWND createWndCheckBox(HWND& parent, const char* str, 
					   int x, int y, int width, int height);
void setWndCheckBox(HWND checkBox, bool on);
bool getWndCheckBox(HWND checkBox);

struct WndComboBox
{
	WndComboBox(HWND& parent, const char** str, int strNum,
				int x, int y, int width, int height);
	~WndComboBox();

	void add(const char** str, int strNum);
	void add(const char* str, int idx);
	void remove(int idx);
	int getCount();
	int getSelectedIdx();
	char* getText(int idx);
	void setSelect(int idx);
	
	void command(WPARAM wParm, LPARAM lParm);

	HWND comboBox;
};


#include"iWindows.h"
#include"iStd.h"
#include"CardGame/GameAttribute.h"

wchar_t* utf8_to_utf16(const char* szFormat, ...)
{
	char szText[1024];
	formating_text(szText, szFormat);
	int len = MultiByteToWideChar(CP_UTF8, 0, szText, -1, NULL, 0);
	wchar_t* wStr = new wchar_t[len];
	MultiByteToWideChar(CP_UTF8, 0, szText, strlen(szText)+1, wStr, len);

	return wStr;
}

char* utf16_to_utf8(const wchar_t* wStr)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, wStr, lstrlenW(wStr), NULL, 0, NULL, NULL);
	char* str = new char[len++];
	WideCharToMultiByte(CP_UTF8, 0, wStr, lstrlenW(wStr) , str, len, NULL, NULL);
	str[len] = 0;

	return str;
}

void start(HDC hdc,SCENE_METHOD m)
{	
	iGraphics::share()->start(hdc);
	loadOpenGL(hdc);
	m();
}

void drawApp(SCENE_UPDATE_METHOD m)
{
	iFPS* f = iFPS::share();
	f->update();
#ifdef SHOW_FPS
	printf("%d\n", f->fps);
#endif // show fps

	m(f->dt);
	iInputManagement::share()->update();
}

void end(SCENE_METHOD m)
{
	m();
	delete iGraphics::share();
	delete iFPS::share();
	delete iInputManagement::share();
	freeOpenGL();
}

void showMessageBox(const char* title, const char* msg)
{
	wchar_t* t = utf8_to_utf16(title);
	wchar_t* m = utf8_to_utf16(msg);
	MessageBox(NULL, m, t, MB_OK);
	delete t;
	delete m;
}

void initWndCtrlSystem()
{
#if 0// 6.0 or later
	InitCommonControls();
#else // 4.7 or later
	INITCOMMONCONTROLSEX ctrl;
	ctrl.dwSize = sizeof(INITCOMMONCONTROLSEX);
	ctrl.dwICC = ICC_UPDOWN_CLASS | ICC_STANDARD_CLASSES;

	InitCommonControlsEx(&ctrl);
#endif
}

HWND createWndStatic(HWND& p, const char* str, int x, int y, int w, int h)
{
	HWND result;
	wchar_t* s = utf8_to_utf16(str);
	HINSTANCE ip = (HINSTANCE)GetWindowLong(p, GWLP_HINSTANCE);
	result = CreateWindow(WC_STATIC, s,
						  WS_CHILD | WS_VISIBLE | ES_CENTER,
						  x, y, w, h,
						  p, (HMENU)0, ip, NULL);

	delete s;

	return result;
}

HWND createWndButton(HWND& p, const char* str, int x, int y, int w, int h)
{
	HWND result;
	wchar_t* s = utf8_to_utf16(str);

	HINSTANCE ip = (HINSTANCE)GetWindowLong(p, GWL_HINSTANCE);
	result = CreateWindow(WC_BUTTON, s,
						  WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
						  x,y,w,h,
						  p,(HMENU)0, ip, NULL);

	delete s;

	return result;
}

HWND createWndCheckBox(HWND& p, const char* str, int x, int y, int w, int h)
{
	HWND result;
	wchar_t* s = utf8_to_utf16(str);

	HINSTANCE ip = (HINSTANCE)GetWindowLong(p, GWL_HINSTANCE);
	result = CreateWindow(WC_BUTTON, s,
						  WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
						  x, y, w, h,
						  p, (HMENU)0, ip, NULL);

	delete s;

	return result;
}

void setWndCheckBox(HWND cb, bool on)
{
	SendMessage(cb, BM_SETCHECK, on ? BST_CHECKED : BST_UNCHECKED, (LPARAM)0);
}

bool getWndCheckBox(HWND cb)
{
	return SendMessage(cb, BM_GETCHECK, (WPARAM)0, (LPARAM)0);
}

WndComboBox::WndComboBox(HWND& p, const char** str, int num, int x, int y, int w, int h)
{
	HINSTANCE ip = (HINSTANCE)GetWindowLong(p, GWL_HINSTANCE);
	comboBox = CreateWindow(WC_COMBOBOX, NULL,
							WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN | CBS_HASSTRINGS,
							x, y, w, h,
							p, (HMENU)0, ip, NULL);

	add(str, num);
	setSelect(0);
}

WndComboBox::~WndComboBox()
{
}

void WndComboBox::add(const char** str, int num)
{
	for (int i = 0; i < num; i++)
	{
		wchar_t* s = utf8_to_utf16(str[i]);
		SendMessage(comboBox, CB_INSERTSTRING, (WPARAM)i, (LPARAM)s);
		delete s;
	}
}

void WndComboBox::add(const char* str, int idx)
{
	wchar_t* s = utf8_to_utf16(str);
	SendMessage(comboBox, CB_INSERTSTRING, (WPARAM)idx, (LPARAM)s);
	delete s;
}

void WndComboBox::remove(int idx)
{
	SendMessage(comboBox, CB_DELETESTRING, (WPARAM)idx, (LPARAM)0);
}

int WndComboBox::getCount()
{
	return SendMessage(comboBox, CB_GETCOUNT, (WPARAM)0, (LPARAM)0);
}

int WndComboBox::getSelectedIdx()
{
	return SendMessage(comboBox, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
}

char* WndComboBox::getText(int idx)
{
	wchar_t r[255];
	SendMessage(comboBox, CB_GETLBTEXT, (WPARAM)idx, (LPARAM)r);
	return utf16_to_utf8(r);
}

void WndComboBox::setSelect(int idx)
{
	SendMessage(comboBox, CB_SETCURSEL, (WPARAM)idx, (LPARAM)0);
}

void WndComboBox::command(WPARAM wParm, LPARAM lParm)
{
	HWND hwnd = (HWND)lParm;

	if (hwnd == comboBox)
	{

	}
}

#include "po.h"
#include "iStd.h"
#include "iWindows.h"

HINSTANCE hInst;                                
WCHAR szTitle[100]=TEXT("CardGame");                  
WCHAR szWindowClass[100]=TEXT("CardGame");
bool isFullscreen;
HWND hWnd;
iRect sizeMonitor;

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PO));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 4);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PO);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);

    hInst = hInstance;

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        0, 0, 1400, 800, nullptr, nullptr, hInstance, nullptr);

    SetMenu(hWnd, NULL);

    if (!hWnd) return FALSE;

    HDC hdc =GetDC(hWnd);
    start(hdc,loadGame);
    isFullscreen = false;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PO));

    MSG msg;

    for (run = true;run;)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            fbo->bind();
            fbo->clear(0, 0, 0, 1);
            drawApp(drawGame);
            fbo->unbind();

            readyOpenGL();
            iTexture* t = fbo->colorBuf;
            iVector2D pos = iVector2DMakeZero;
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            drawImage(t, pos, BOTTOM | RIGHT,
                iVector2DMake(0, 0), iVector2DMake(t->width, t->height),
                iVector2DMake(1,1),
                2, 0 , REVERSE_HEIGHT);
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                                GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            
            SwapBuffers(hdc);
        }
    }

    end(freeGame);
    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
        {
            sizeMonitor.size.x = GetSystemMetrics(SM_CXSCREEN);
            sizeMonitor.size.y = GetSystemMetrics(SM_CYSCREEN);
            return 0;
        }
        case WM_LBUTTONDOWN:
        {
            iVector2D pos = 
            coordinate(iVector2DMake(LOWORD(lParam), HIWORD(lParam)));
            iInputManagement::share()->mouseUpdate(MOUSE_LDOWN, pos);
            return 0;
        }
        case WM_MOUSEMOVE:
        {
            iVector2D pos =
            coordinate(iVector2DMake(LOWORD(lParam), HIWORD(lParam)));
            iInputManagement::share()->mouseUpdate(MOUSE_MOVE, pos);
            return 0;
        }
        case WM_LBUTTONUP:
        {
            iVector2D pos =
            coordinate(iVector2DMake(LOWORD(lParam), HIWORD(lParam)));
            iInputManagement::share()->mouseUpdate(MOUSE_LUP, pos);
            return 0;
        }
        case WM_KEYDOWN:
        {
            iInputManagement::share()->keyUpdate(KeySateDown, wParam);

            if (wParam == VK_ESCAPE)
            {
                void goFullscreen();
                goFullscreen();
            }

            return 0;
        }
        case WM_KEYUP:
        {
            iInputManagement::share()->keyUpdate(KeyStateUP, wParam);
            return 0;
        }
        case WM_SIZE:
        {
            //getViewPort(LOWORD(lParam), HIWORD(lParam));
            //return 0;
            RECT rt;
            GetClientRect(hWnd, &rt);

            float curW = rt.right - rt.left;
            float curH = rt.bottom - rt.top;

            float ratio = (float)DEV_WIDTH / (float)DEV_HEIGHT;

            float h = curH;
            float w = h * ratio;
            int posX, posY;

            if (w < curW)
            {
                posX = (curW - w) / 2;
                posY = 0;
                viewPort.origin = iVector2DMake(posX, posY);
                viewPort.size = iVector2DMake(w, h);
                return 0;
            }

            w = curW;
            h = w / ratio;

            if (h < curH)
            {
                posX = 0;
                posY = (curH - h) / 2;
                viewPort.origin = iVector2DMake(posX, posY);
                viewPort.size = iVector2DMake(w, h);
                return 0;
            }

            return 0;
        }
        case WM_CLOSE :
        {
            const char* str[6] =
            {
                "hello" , "close?",
                "안녕","끌래?",
                "--","**"
            };

            const wchar_t* str1 = utf8_to_utf16(str[LANGUAGE]);
            const wchar_t* str2 = utf8_to_utf16(str[LANGUAGE+1]);
            
            if (MessageBox(NULL, str1, str2, MB_YESNO) == IDYES)
                run = false;
            
            delete str2;
            delete str1;
            return 0;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

void setCurrentMonitor(RECT& rt)
{
    GetWindowRect(hWnd, &rt);
    HMONITOR hMonitor = MonitorFromRect(&rt, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi;
    mi.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(hMonitor, &mi);
    if (mi.dwFlags == MONITORINFOF_PRIMARY)
    {
        rt.left = 0;
        rt.right = sizeMonitor.size.x;
        rt.top = 0;
        rt.bottom = sizeMonitor.size.y;
    }
    else
    {
        memcpy(&rt, &mi.rcWork, sizeof(RECT));
    }
}

RECT rtPrev;
void goFullscreen()
{
    isFullscreen = !isFullscreen;
    if (isFullscreen)
    {
        GetWindowRect(hWnd, &rtPrev);
        RECT rt;
        setCurrentMonitor(rt);
        int x = rt.left,
            y = rt.top,
            w = rt.right - rt.left,
            h = rt.bottom - rt.top;
        SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
        SetWindowPos(hWnd, HWND_TOP,
            x, y, w, h,
            SWP_SHOWWINDOW);
    }
    else
    {
        int x = rtPrev.left,
            y = rtPrev.top,
            w = rtPrev.right - rtPrev.left,
            h = rtPrev.bottom - rtPrev.top;
        SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
        SetWindowPos(hWnd, HWND_TOP,
            x, y, w, h, SWP_SHOWWINDOW);
    }
}

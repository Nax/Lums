#include <mutex>
#include <windows.h>
#include <Lums/Window/Window.h>

using namespace lm;

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

namespace
{

ATOM        gWinClass;
int         gWinClassRefCount;
std::mutex  gWinClassMutex;

LRESULT CALLBACK WindowProc(
  _In_ HWND   hwnd,
  _In_ UINT   uMsg,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
)
{
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

}

Window::Window(const char* title, int width, int height)
{
    /* We need this lock to ensure the thread-safe management of the window class */
    std::unique_lock<std::mutex> lock{gWinClassMutex};

    /* Register the class if required, and increment the refcount */
    if (!gWinClassRefCount)
    {
        WNDCLASSEX wc{};
        wc.cbSize = sizeof(wc);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = &WindowProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = (HINSTANCE)&__ImageBase;
        wc.hIcon = nullptr;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = nullptr;
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = TEXT("LumsWindow");
        wc.hIconSm = nullptr;

        gWinClass = RegisterClassEx(&wc);
    }
    gWinClassRefCount++;
    lock.unlock();

    /* Convert the window title */
    TCHAR titleBuf[512];
    MultiByteToWideChar(CP_UTF8, 0, title, -1, titleBuf, sizeof(titleBuf) / sizeof(*titleBuf));

    /* Create the window */
    _win = CreateWindowEx(
        0,
        (LPCTSTR)gWinClass,
        titleBuf,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        nullptr,
        nullptr,
        nullptr,
        this
    );

    /* Get the DC */
    _dc = GetDC((HWND)_win);
}

Window::~Window()
{
    /* Release the DC */
    ReleaseDC((HWND)_win, (HDC)_dc);

    /* Destroy the window */
    DestroyWindow((HWND)_win);

    {
        std::unique_lock<std::mutex> lock{gWinClassMutex};
        if (--gWinClassRefCount == 0)
        {
            UnregisterClass((LPCTSTR)gWinClass, (HINSTANCE)&__ImageBase);
        }
    }
}

Vector2i Window::size() const
{
    RECT r;
    GetClientRect((HWND)_win, &r);
    return Vector2i(r.right - r.left, r.bottom - r.top);
}

void Window::show()
{
    ShowWindow((HWND)_win, SW_SHOW);
}

void Window::hide()
{
    ShowWindow((HWND)_win, SW_HIDE);
}

bool Window::poll()
{
    MSG msg;

    for (;;)
    {
        if (!PeekMessage(&msg, (HWND)_win, 0, 0, PM_REMOVE))
            return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

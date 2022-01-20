#include <cstdio>
#include <Lums/Window/Window.h>
#include <Lums/Draw/DrawRendererOpenGL.h>
#include <Lums/OpenGL/GL.h>

using namespace lm;

namespace
{

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

LRESULT CALLBACK WindowProcFake(
  _In_ HWND   hwnd,
  _In_ UINT   uMsg,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
)
{
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

const PIXELFORMATDESCRIPTOR kPixelFormatDescriptor = {
    sizeof(kPixelFormatDescriptor),
    1,
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    32,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
    0,
    24,
    8,
    0,
    PFD_MAIN_PLANE,
    0, 0, 0, 0
};

const int kPixelFormatAttribList[] = {
    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
    WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    WGL_COLOR_BITS_ARB, 32,
    WGL_DEPTH_BITS_ARB, 24,
    WGL_STENCIL_BITS_ARB, 8,
    0
};

const int kContextAttribList[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
    WGL_CONTEXT_MINOR_VERSION_ARB, 2,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    0
};

bool bootstrapped;

/* WGL */
PFNWGLCHOOSEPIXELFORMATARBPROC      wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC   wglCreateContextAttribsARB;

#define LOAD_GL(x)  do { (*((void**)(&x))) = getOpenGLProcAddr(#x); } while (0)

void boostrapOpenGL()
{
    ATOM    fakeClass;
    HWND    fakeWin;
    HDC     fakeDC;
    HGLRC   fakeContext;

    /* Create the fake class */
    WNDCLASSEX wc{};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = &WindowProcFake;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = (HINSTANCE)&__ImageBase;
    wc.hIcon = nullptr;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = TEXT("LumsFakeWindow");
    wc.hIconSm = nullptr;
    fakeClass = RegisterClassEx(&wc);

    /* Create a fake window */
    fakeWin = CreateWindowEx(
        0,
        (LPCTSTR)fakeClass,
        TEXT(""),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );

    /* Get the fake DC */
    fakeDC = GetDC(fakeWin);

    /* Apply our fake pixel format */
    int pf = ChoosePixelFormat(fakeDC, &kPixelFormatDescriptor);
    SetPixelFormat(fakeDC, pf, &kPixelFormatDescriptor);

    /* Create and apply the fake context */
    fakeContext = wglCreateContext(fakeDC);
    wglMakeCurrent(fakeDC, fakeContext);

    /* Get the WGL extensions */
    LOAD_GL(wglChoosePixelFormatARB);
    LOAD_GL(wglCreateContextAttribsARB);

    /* Destroy the fake context */
    wglMakeCurrent(fakeDC, nullptr);
    wglDeleteContext(fakeContext);

    /* Destroy the fake DC */
    ReleaseDC(fakeWin, fakeDC);

    /* Destroy the fake window */
    DestroyWindow(fakeWin);

    /* Unregister the fake class */
    UnregisterClass((LPCTSTR)fakeClass, (HINSTANCE)&__ImageBase);

    bootstrapped = true;
}

}

void DrawRendererOpenGL::platformCreateContext()
{
    HGLRC context;

    /* Bootstrap OpenGL */
    if (!bootstrapped)
        boostrapOpenGL();

    int pixelFormat{};
    UINT numFormats{};

    /* Set our pixel format */
    wglChoosePixelFormatARB(_window.dc(), kPixelFormatAttribList, nullptr, 1, &pixelFormat, &numFormats);
    SetPixelFormat(_window.dc(), pixelFormat, &kPixelFormatDescriptor);

    /* Create the OpenGL context */
    context = wglCreateContextAttribsARB(_window.dc(), nullptr, kContextAttribList);
    wglMakeCurrent(_window.dc(), context);

    _context = context;
}

void DrawRendererOpenGL::platformDestroyContext()
{
    HGLRC context = (HGLRC)_context;

    if (context)
    {
        wglMakeCurrent(_window.dc(), nullptr);
        wglDeleteContext(context);
    }
}

void DrawRendererOpenGL::platformSwap()
{
    SwapBuffers(_window.dc());
}

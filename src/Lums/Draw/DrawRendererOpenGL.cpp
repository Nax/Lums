#include <cstdio>
#include <Lums/Draw/DrawRendererOpenGL.h>
#include <Lums/Window/Window.h>
#include <Lums/OpenGL/GL.h>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

using namespace lm;

namespace
{

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

PFNWGLCHOOSEPIXELFORMATARBPROC      wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC   wglCreateContextAttribsARB;

/* Framebuffer */
PFNGLGENFRAMEBUFFERSPROC    glGenFramebuffers;
PFNGLBINDFRAMEBUFFERPROC    glBindFramebuffer;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;
PFNGLDRAWBUFFERSPROC        glDrawBuffers;

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

GLenum texType2gl(DrawTextureType t)
{
    switch (t)
    {
    case DrawTextureType::Texture2D:
        return GL_TEXTURE_2D;
    }
    return 0;
}

GLenum texFormat2gl(DrawTextureFormat t)
{
    switch (t)
    {
    case DrawTextureFormat::RGBA:
        return GL_RGBA;
    }
    return 0;
}

GLint texFormat2glInternal(DrawTextureFormat t)
{
    switch (t)
    {
    case DrawTextureFormat::RGBA:
        return GL_RGBA8;
    }
    return 0;
}

}

DrawRendererOpenGL::DrawRendererOpenGL(Window& win)
: DrawRenderer(win)
, _context{}
{
    /* Bootstrap OpenGL */
    if (!bootstrapped)
        boostrapOpenGL();

    int pixelFormat{};
    UINT numFormats{};

    /* Set our pixel format */
    wglChoosePixelFormatARB(win.dc(), kPixelFormatAttribList, nullptr, 1, &pixelFormat, &numFormats);
    SetPixelFormat(win.dc(), pixelFormat, &kPixelFormatDescriptor);

    /* Create the OpenGL context */
    _context = wglCreateContextAttribsARB(win.dc(), nullptr, kContextAttribList);
    wglMakeCurrent(win.dc(), _context);

    /* Print the context infos */
    std::printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    std::printf("OpenGL vendor : %s\n", glGetString(GL_VENDOR));

    /* Load the required GL funcs */
    LOAD_GL(glGenFramebuffers);
    LOAD_GL(glBindFramebuffer);
    LOAD_GL(glDeleteFramebuffers);
    LOAD_GL(glFramebufferTexture);
    LOAD_GL(glDrawBuffers);

    /* Register handlers */
#define HANDLER(member)                 ([](void* r, const priv::DrawCommand* cmd) { ((DrawRendererOpenGL*)r)->member(cmd); })
#define REGISTER_HANDLER(cmd, member)   _handlers[(int)priv::DrawCommandType::cmd] = HANDLER(member)

    REGISTER_HANDLER(CreateTexture, implCreateTexture);
    REGISTER_HANDLER(DestroyTexture, implDestroyTexture);
    REGISTER_HANDLER(CreateFramebuffer, implCreateFramebuffer);
    REGISTER_HANDLER(DestroyFramebuffer, implDestroyFramebuffer);

#undef REGISTER_HANDLER
#undef HANDLER
}

DrawRendererOpenGL::~DrawRendererOpenGL()
{
    if (_context)
    {
        wglMakeCurrent(_window.dc(), nullptr);
        wglDeleteContext(_context);
    }
}

bool DrawRendererOpenGL::valid() const
{
    return !!_context;
}

void DrawRendererOpenGL::swap()
{
    SwapBuffers(_window.dc());
}

void DrawRendererOpenGL::implCreateTexture(const priv::DrawCommand* cmd)
{
    auto i = cmd->texture.texture.value();
    auto target = texType2gl(cmd->texture.type);
    auto format = texFormat2gl(cmd->texture.format);
    auto internalFormat = texFormat2glInternal(cmd->texture.format);

    GLuint tex;
    if (_textures.size() < i)
        _textures.resize(i);
    glGenTextures(1, &tex);
    glBindTexture(target, tex);

    switch (target)
    {
    case GL_TEXTURE_2D:
        glTexImage2D(target, 0, internalFormat, cmd->texture.width, cmd->texture.height, 0, format, GL_UNSIGNED_BYTE, cmd->texture.data);
        break;
    }

    std::free(cmd->texture.data);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    _textures[i] = tex;
}

void DrawRendererOpenGL::implDestroyTexture(const priv::DrawCommand* cmd)
{
    auto i = cmd->texture.texture.value();
    glDeleteTextures(1, &_textures[i]);
    _textures[i] = 0;
}

void DrawRendererOpenGL::implCreateFramebuffer(const priv::DrawCommand* cmd)
{
    auto i = cmd->framebuffer.framebuffer.value();

    GLuint fb;
    GLuint colors[priv::kMaxFramebufferColors] = {0};

    if (_framebuffers.size() < i)
        _framebuffers.resize(i);

    glGenFramebuffers(1, &fb);
    glBindFramebuffer(GL_FRAMEBUFFER, fb);

    for (int j = 0; j < priv::kMaxFramebufferColors; ++j)
    {
        if (cmd->framebuffer.color[i])
        {
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + j, _textures[cmd->framebuffer.color[j].value()], 0);
            colors[i] = GL_COLOR_ATTACHMENT0 + j;
        }
    }
    glDrawBuffers(priv::kMaxFramebufferColors, colors);

    if (cmd->framebuffer.depth)
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _textures[cmd->framebuffer.depth.value()], 0);

    if (cmd->framebuffer.stencil)
        glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, _textures[cmd->framebuffer.stencil.value()], 0);

    _framebuffers[i] = fb;
}

void DrawRendererOpenGL::implDestroyFramebuffer(const priv::DrawCommand* cmd)
{
    auto i = cmd->framebuffer.framebuffer.value();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, _framebuffers.data() + i);
    _framebuffers[i] = 0;
}

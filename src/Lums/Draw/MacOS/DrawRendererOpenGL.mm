#import <Cocoa/Cocoa.h>
#import <Lums/Draw/DrawRendererOpenGL.h>
#import <Lums/Window/Window.h>

using namespace lm;

static const NSOpenGLPixelFormatAttribute kAttributes[] = {
    NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFAAccelerated,
    NSOpenGLPFAColorSize, 24,
    NSOpenGLPFAAlphaSize, 8,
    NSOpenGLPFADepthSize, 24,
    NSOpenGLPFAStencilSize, 8,
    0
};

void DrawRendererOpenGL::platformCreateContext()
{
    NSWindow* win = (NSWindow*)_window.handle();
    NSRect rect = NSMakeRect(0.f, 0.f, (float)_window.size().x, (float)_window.size().y);
    NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:kAttributes];
    NSOpenGLView* view = [[NSOpenGLView alloc] initWithFrame:rect pixelFormat:pixelFormat];
    NSOpenGLContext* ctx = [[view openGLContext] retain];

    [win setContentView:view];
    [ctx makeCurrentContext];
    [view prepareOpenGL];

    [pixelFormat release];
    [view release];

    _context = ctx;
}

void DrawRendererOpenGL::platformDestroyContext()
{
    [NSOpenGLContext clearCurrentContext];
    [(NSOpenGLContext*)_context release];
}

void DrawRendererOpenGL::platformSwap()
{
    [(NSOpenGLContext*)_context flushBuffer];
}

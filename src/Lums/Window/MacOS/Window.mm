#import <Lums/Window/Window.h>

using namespace lm;

Window::Window(const char* title, int width, int height)
{
    _size.x = width;
    _size.y = height;

    NSRect frame = NSMakeRect(0, 0, width, height);
    NSWindow* win = [[NSWindow alloc]
        initWithContentRect:frame
        styleMask:NSWindowStyleMaskTitled
        backing:NSBackingStoreBuffered
        defer:YES
    ];
    [win setOpaque:YES];
    [win setTitle:[NSString stringWithUTF8String:title]];
    _handle = (void*)win;
}

Window::~Window()
{
    [(NSWindow*)_handle release];
}

void Window::show()
{
    [(NSWindow*)_handle makeKeyAndOrderFront:nil];
}

void Window::hide()
{
    [(NSWindow*)_handle orderOut:nil];
}

bool Window::poll()
{
    NSApplication* app = [NSApplication sharedApplication];
    NSEvent* event;

    for (;;)
    {
        event = [app
            nextEventMatchingMask:NSEventMaskAny
            untilDate:nil
            inMode:NSDefaultRunLoopMode
            dequeue:YES
        ];
        if (event == nil)
            break;
        [NSApp sendEvent:event];
    }

    return false;
}

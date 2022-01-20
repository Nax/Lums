#import <Lums/Window/Window.h>

using namespace lm;

Window::Window(const char* title, int width, int height)
{
    NSRect frame = NSMakeRect(0, 0, width, height);
    _win = [[NSWindow alloc]
        initWithContentRect:frame
        styleMask:NSWindowStyleMaskTitled
        backing:NSBackingStoreBuffered
        defer:YES
    ];
    [_win setTitle:[NSString stringWithUTF8String:title]];
}

Window::~Window()
{
}

Vector2i Window::size() const
{

}

void Window::show()
{
    [_win makeKeyAndOrderFront:nil];
}

void Window::hide()
{
    [_win orderOut:nil];
}

bool Window::poll()
{

}

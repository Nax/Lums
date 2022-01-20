#ifndef LUMS_INCLUDED_WINDOW_WINDOW_H
#define LUMS_INCLUDED_WINDOW_WINDOW_H

#include <Lums/Window/ABI.h>
#include <Lums/Math/Vector.h>
#include <Lums/Util/NonCopyable.h>

#if defined(LUMS_OS_WINDOWS)
# include <windows.h>
#endif

#if defined(LUMS_OS_MACOS) && defined(__OBJC__)
# include <Cocoa/Cocoa.h>
#endif

namespace lm
{

/**
 * Window
 *
 */
class LUMS_API_WINDOW Window : private NonCopyable
{
public:
    Window(const char* title, int width, int height);
    ~Window();

    Vector2i size() const;

#if defined(LUMS_OS_WINDOWS)
    HWND    hwnd() const;
    HDC     dc() const;
#endif

    void show();
    void hide();
    bool poll();

private:
#if defined(LUMS_OS_WINDOWS)
    HWND    _win;
    HDC     _dc;
#endif

#if defined(LUMS_OS_MACOS)
# if !(defined(__OBJC__))
    using NSWindow = void;
# endif
    NSWindow* _win;
#endif
};

}

#endif /* LUMS_INCLUDED_WINDOW_WINDOW_H */

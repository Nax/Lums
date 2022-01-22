#ifndef LUMS_INCLUDED_WINDOW_WINDOW_H
#define LUMS_INCLUDED_WINDOW_WINDOW_H

#include <Lums/Math/Vector.h>
#include <Lums/Util/ABI.h>
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
class LUMS_API Window : private NonCopyable
{
public:
    Window(const char* title, int width, int height);
    ~Window();

    const void* handle() const;
    void*       handle();
    Vector2i    size() const;

    void show();
    void hide();
    bool poll();

private:
    void*       _handle;
    Vector2i    _size;
};

}

#endif /* LUMS_INCLUDED_WINDOW_WINDOW_H */

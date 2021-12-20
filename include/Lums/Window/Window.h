#ifndef LUMS_INCLUDED_WINDOW_WINDOW_H
#define LUMS_INCLUDED_WINDOW_WINDOW_H

#include <Lums/Window/ABI.h>
#include <Lums/Math/Vector.h>
#include <Lums/Util/NonCopyable.h>

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

    void show();
    void hide();
    bool poll();

private:
#if defined(LUMS_OS_WINDOWS)
    void*       _win;
    void*       _dc;
#endif
};

}

#endif /* LUMS_INCLUDED_WINDOW_WINDOW_H */

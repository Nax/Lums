#ifndef LUMS_INCLUDED_WINDOW_WINDOW_H
#define LUMS_INCLUDED_WINDOW_WINDOW_H

#include <Lums/Window/ABI.h>
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
    Window();
    virtual ~Window();

    virtual void swap() = 0;

protected:
#if defined(LUMS_OS_WINDOWS)
    void*   _win;
    void*   _dc;
#endif
};

}

#endif /* LUMS_INCLUDED_WINDOW_WINDOW_H */

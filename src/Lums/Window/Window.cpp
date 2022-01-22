#include <Lums/Window/Window.h>

using namespace lm;

const void* Window::handle() const
{
    return _handle;
}

void* Window::handle()
{
    return _handle;
}

Vector2i Window::size() const
{
    return _size;
}

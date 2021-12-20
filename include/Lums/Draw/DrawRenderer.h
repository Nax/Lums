#ifndef LUMS_INCLUDED_DRAW_DRAW_RENDERER_H
#define LUMS_INCLUDED_DRAW_DRAW_RENDERER_H

#include <Lums/Util/NonCopyable.h>
#include <Lums/Util/SafeHandleAllocator.h>
#include <Lums/Draw/ABI.h>
#include <Lums/Draw/DrawCommand.h>
#include <Lums/Draw/Types.h>

namespace lm
{

class Window;
class DrawRenderer : private NonCopyable
{
public:
    LUMS_API_DRAW DrawRenderer(Window& window);
    LUMS_API_DRAW virtual ~DrawRenderer() = 0;

    LUMS_API_DRAW virtual bool valid() const = 0;

    LUMS_API_DRAW DrawFramebuffer createFramebuffer();
    LUMS_API_DRAW void            destroyFramebuffer(DrawFramebuffer fb);

    LUMS_API_DRAW void render();
    LUMS_API_DRAW virtual void swap() = 0;

protected:
    enum class CommandType
    {
        CreateFramebuffer,
        DestroyFramebuffer,
    };

    Window& _window;

    priv::DrawCommandHandler        _handlers[32];
    std::vector<priv::DrawCommand>  _commands;

    SafeHandleAllocator<DrawFramebuffer>    _allocFramebuffer;
};

}

#endif /* LUMS_INCLUDED_DRAW_DRAW_RENDERER_H */

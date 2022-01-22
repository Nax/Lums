#ifndef LUMS_INCLUDED_DRAW_DRAW_RENDERER_H
#define LUMS_INCLUDED_DRAW_DRAW_RENDERER_H

#include <Lums/Util/NonCopyable.h>
#include <Lums/Util/SafeHandleAllocator.h>
#include <Lums/Util/ABI.h>
#include <Lums/Draw/DrawCommand.h>
#include <Lums/Draw/Types.h>
#include <Lums/Math/Vector.h>

namespace lm
{

class Window;
class DrawRenderer : private NonCopyable
{
public:
    LUMS_API DrawRenderer(Window& window);
    LUMS_API virtual ~DrawRenderer() = 0;

    LUMS_API virtual bool valid() const = 0;

    LUMS_API DrawShader        createShader(const char* vertexSrc, const char* fragmentSrc);
    LUMS_API DrawTexture       createTexture(DrawTextureType type, DrawTextureFormat fmt, int width, int height, void* data);
    LUMS_API DrawFramebuffer   createFramebuffer(DrawTexture* colors, int colorNum, DrawTexture depth, DrawTexture stencil);

    LUMS_API void  destroyShader(DrawShader shader);
    LUMS_API void  destroyTexture(DrawTexture tex);
    LUMS_API void  destroyFramebuffer(DrawFramebuffer fb);

    LUMS_API void  clear(DrawFramebuffer fb, int flags, Vector4f color, float depth, int stencil);
    LUMS_API void  draw(const DrawList& list);

    LUMS_API void render();

    static LUMS_API DrawRenderer* create(Window& win);

protected:
    LUMS_API virtual void swap() = 0;

    Window& _window;

    priv::DrawCommandHandler        _handlers[32];
    std::vector<priv::DrawCommand>  _commands;

    SafeHandleAllocator<DrawShader>         _allocShader;
    SafeHandleAllocator<DrawTexture>        _allocTexture;
    SafeHandleAllocator<DrawFramebuffer>    _allocFramebuffer;
};

}

#endif /* LUMS_INCLUDED_DRAW_DRAW_RENDERER_H */

#ifndef LUMS_INCLUDED_DRAW_DRAW_RENDERER_OPENGL_H
#define LUMS_INCLUDED_DRAW_DRAW_RENDERER_OPENGL_H

#include <Lums/Draw/DrawRenderer.h>
#include <Lums/OpenGL/GL.h>

namespace lm
{

class DrawRendererOpenGL : public DrawRenderer
{
public:
    LUMS_API_DRAW DrawRendererOpenGL(Window& win);
    LUMS_API_DRAW ~DrawRendererOpenGL();

    LUMS_API_DRAW bool valid() const override;
    LUMS_API_DRAW void swap() override;

    void implCreateFramebuffer(const priv::DrawCommand* cmd);
    void implDestroyFramebuffer(const priv::DrawCommand* cmd);

private:
    HGLRC _context;
};

}

#endif /* LUMS_INCLUDED_DRAW_DRAW_RENDERER_OPENGL_H */

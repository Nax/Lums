#ifndef LUMS_INCLUDED_DRAW_DRAW_RENDERER_OPENGL_H
#define LUMS_INCLUDED_DRAW_DRAW_RENDERER_OPENGL_H

#include <Lums/Draw/DrawRenderer.h>
#include <Lums/OpenGL/GL.h>

namespace lm
{

class DrawRendererOpenGL : public DrawRenderer
{
public:
    LUMS_API DrawRendererOpenGL(Window& win);
    LUMS_API ~DrawRendererOpenGL();

    LUMS_API bool valid() const override;

    void implCreateShader(const priv::DrawCommand* cmd);
    void implDestroyShader(const priv::DrawCommand* cmd);
    void implCreateTexture(const priv::DrawCommand* cmd);
    void implDestroyTexture(const priv::DrawCommand* cmd);
    void implCreateFramebuffer(const priv::DrawCommand* cmd);
    void implDestroyFramebuffer(const priv::DrawCommand* cmd);
    void implClear(const priv::DrawCommand* cmd);
    void implDraw(const priv::DrawCommand* cmd);

protected:
    LUMS_API void swap() override;

private:
    void platformCreateContext();
    void platformDestroyContext();
    void platformSwap();

#if defined(__OBJC__)
    NSOpenGLContext*    _context;
#else
    void*               _context;
#endif

    std::vector<GLuint> _programs;
    std::vector<GLuint> _textures;
    std::vector<GLuint> _framebuffers;

    Vector4f    _clearColor;
    float       _clearDepth;
    int         _clearStencil;
};

}

#endif /* LUMS_INCLUDED_DRAW_DRAW_RENDERER_OPENGL_H */

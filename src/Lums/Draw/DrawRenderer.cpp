#include <Lums/Draw/DrawRenderer.h>

using namespace lm;

DrawRenderer::DrawRenderer(Window& window)
: _window{window}
, _handlers{}
{

}

DrawRenderer::~DrawRenderer()
{

}

DrawFramebuffer DrawRenderer::createFramebuffer()
{
    DrawFramebuffer fb = _allocFramebuffer.alloc();

    priv::DrawCommand cmd;
    cmd.handler = _handlers[(int)priv::DrawCommandType::CreateFramebuffer];
    cmd.framebuffer.framebuffer = fb;
    _commands.push_back(cmd);

    return fb;
}

void DrawRenderer::destroyFramebuffer(DrawFramebuffer fb)
{
    _allocFramebuffer.dealloc(fb);

    priv::DrawCommand cmd;
    cmd.handler = _handlers[(int)priv::DrawCommandType::DestroyFramebuffer];
    cmd.framebuffer.framebuffer = fb;
    _commands.push_back(cmd);
}

void DrawRenderer::render()
{
    /* Run the commands */
    for (const auto& cmd : _commands)
        cmd.handler(this, &cmd);

    /* Call the renderer-defined swap func */
    swap();

    /* Clean up */
    _commands.clear();
}

#include <Lums/Draw/DrawRenderer.h>

using namespace lm;

namespace
{

int formatSize(DrawTextureFormat fmt)
{
    switch (fmt)
    {
    case DrawTextureFormat::RGBA:
        return 4;
    }
    return 0;
}

}

DrawRenderer::DrawRenderer(Window& window)
: _window{window}
, _handlers{}
{

}

DrawRenderer::~DrawRenderer()
{

}

DrawTexture DrawRenderer::createTexture(DrawTextureType type, DrawTextureFormat fmt, int width, int height, void* data)
{
    DrawTexture tex = _allocTexture.alloc();
    void* buf = std::calloc(1, width * height * formatSize(fmt));
    std::memcpy(buf, data, width * height * formatSize(fmt));

    priv::DrawCommand cmd;
    cmd.handler = _handlers[(int)priv::DrawCommandType::CreateTexture];
    cmd.texture.texture = tex;
    cmd.texture.type = type;
    cmd.texture.format = fmt;
    cmd.texture.width = width;
    cmd.texture.height = height;
    cmd.texture.data = buf;
    _commands.push_back(cmd);

    return tex;
}

DrawFramebuffer DrawRenderer::createFramebuffer(DrawTexture* colors, int colorNum, DrawTexture depth, DrawTexture stencil)
{
    DrawFramebuffer fb = _allocFramebuffer.alloc();
    priv::DrawCommand cmd;

    static const int maxColors = sizeof(cmd.framebuffer.color) / sizeof(*cmd.framebuffer.color);

    cmd.handler = _handlers[(int)priv::DrawCommandType::CreateFramebuffer];
    cmd.framebuffer.framebuffer = fb;
    cmd.framebuffer.depth = depth;
    cmd.framebuffer.stencil = stencil;

    int i;
    for (i = 0; i < colorNum; ++i)
        cmd.framebuffer.color[i] = colors[i];
    for (; i < maxColors; ++i)
        cmd.framebuffer.color[i] = DrawTexture();

    _commands.push_back(cmd);

    return fb;
}

void DrawRenderer::destroyTexture(DrawTexture tex)
{
    _allocTexture.dealloc(tex);

    priv::DrawCommand cmd;
    cmd.handler = _handlers[(int)priv::DrawCommandType::DestroyTexture];
    cmd.texture.texture = tex;
    _commands.push_back(cmd);
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

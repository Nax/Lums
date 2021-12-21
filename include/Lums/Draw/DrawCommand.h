#ifndef LUMS_INCLUDED_DRAW_COMMAND_H
#define LUMS_INCLUDED_DRAW_COMMAND_H

#include <Lums/Draw/Types.h>
#include <Lums/Draw/ABI.h>

namespace lm
{

enum class DrawTextureType
{
    Texture2D,
};

enum class DrawTextureFormat
{
    RGBA,
};

namespace priv
{

struct DrawCommand;
using DrawCommandHandler = void(*)(void*, const DrawCommand*);

static const int kMaxFramebufferColors = 4;

enum class DrawCommandType
{
    CreateShader,
    DestroyShader,
    CreateTexture,
    DestroyTexture,
    CreateFramebuffer,
    DestroyFramebuffer,
};

struct DrawCommandShader
{
    DrawShader  shader;
    char*       vertexSrc;
    char*       fragmentSrc;
};

struct DrawCommandTexture
{
    DrawTexture         texture;
    DrawTextureType     type;
    DrawTextureFormat   format;
    int                 width;
    int                 height;
    void*               data;
};

struct DrawCommandFramebuffer
{
    DrawFramebuffer framebuffer;
    DrawTexture     color[kMaxFramebufferColors];
    DrawTexture     depth;
    DrawTexture     stencil;
};

struct DrawCommand
{
    DrawCommandHandler handler;
    union
    {
        DrawCommandShader       shader;
        DrawCommandTexture      texture;
        DrawCommandFramebuffer  framebuffer;
    };

    DrawCommand() {}
};

}

}

#endif /* LUMS_INCLUDED_DRAW_COMMAND_H */

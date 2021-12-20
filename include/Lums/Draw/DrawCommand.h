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
    CreateTexture,
    DestroyTexture,
    CreateFramebuffer,
    DestroyFramebuffer,
};

struct DrawCommandFramebuffer
{
    DrawFramebuffer framebuffer;
    DrawTexture     color[kMaxFramebufferColors];
    DrawTexture     depth;
    DrawTexture     stencil;
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

struct DrawCommand
{
    DrawCommandHandler handler;
    union
    {
        DrawCommandTexture      texture;
        DrawCommandFramebuffer  framebuffer;
    };

    DrawCommand() {}
};

}

}

#endif /* LUMS_INCLUDED_DRAW_COMMAND_H */

#ifndef LUMS_INCLUDED_DRAW_COMMAND_H
#define LUMS_INCLUDED_DRAW_COMMAND_H

#include <Lums/Math/Vector.h>
#include <Lums/Draw/Types.h>
#include <Lums/Draw/ABI.h>

namespace lm
{

class DrawList;

enum class DrawTextureType
{
    Texture2D,
};

enum class DrawTextureFormat
{
    RGBA,
};

namespace DrawClearMask
{

enum
{
    Color = 0x01,
    Depth = 0x02,
    Stencil = 0x04,
};

}

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
    Clear,
    Draw,
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

struct DrawCommandClear
{
    DrawFramebuffer     framebuffer;
    int                 flags;
    Vector4f            color;
    float               depth;
    int                 stencil;
};

struct DrawCommandDraw
{
    const DrawList* list;
};

struct DrawCommand
{
    DrawCommandHandler handler;
    union
    {
        DrawCommandShader       shader;
        DrawCommandTexture      texture;
        DrawCommandFramebuffer  framebuffer;
        DrawCommandClear        clear;
        DrawCommandDraw         draw;
    };

    DrawCommand() {}
};

}

}

#endif /* LUMS_INCLUDED_DRAW_COMMAND_H */

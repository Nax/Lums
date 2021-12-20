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

enum class DrawCommandType
{
    CreateFramebuffer,
    DestroyFramebuffer,
};

struct DrawCommandFramebuffer
{
    DrawFramebuffer framebuffer;
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
    DrawCommand() {}

    DrawCommandHandler handler;
    union
    {
        DrawCommandFramebuffer framebuffer;
    };
};

}

}

#endif /* LUMS_INCLUDED_DRAW_COMMAND_H */

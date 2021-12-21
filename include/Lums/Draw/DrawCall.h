#ifndef LUMS_INCLUDED_DRAW_DRAW_CALL_H
#define LUMS_INCLUDED_DRAW_DRAW_CALL_H

#include <cstdint>
#include <Lums/Draw/Types.h>

namespace lm
{

enum class DrawDepthFunc : std::uint8_t
{
    Never = 0,
    Less = 1,
    Equal = 2,
    LessEqual = 3,
    Greater = 4,
    NotEqual = 5,
    GreaterEqual = 6,
    Always = 7,
};

static const int kMaxDrawCallTextureCount = 4;

struct DrawCall
{
    bool            depthWrite:1;
    DrawDepthFunc   depthFunc:3;
    std::uint8_t    layer;
    std::uint8_t    sublayer;
    DrawShader      shader;
    DrawTexture     textures[kMaxDrawCallTextureCount];

    DrawCall()
    : depthWrite{true}
    , depthFunc{DrawDepthFunc::Less}
    , layer{}
    , sublayer{}
    {
    }

    std::uint64_t sortKey() const
    {
        return (((std::uint64_t)layer) << 56)
            | (((std::uint64_t)sublayer) << 48)
            | (((std::uint64_t)shader.value()) << 32)
            | (((std::uint64_t)textures[0].value()) << 16)
            | (((std::uint64_t)textures[1].value()) << 0);
    }
};

bool operator<(const DrawCall& a, const DrawCall& b)
{
    return a.sortKey() < b.sortKey();
}

}

#endif /* LUMS_INCLUDED_DRAW_DRAW_CALL_H */

#ifndef LUMS_INCLUDED_DRAW_TYPES_H
#define LUMS_INCLUDED_DRAW_TYPES_H

#include <cstdint>
#include <Lums/Util/SafeHandle.h>

namespace lm
{

namespace priv
{
    struct DrawShaderTag {};
    struct DrawTextureTag {};
    struct DrawFramebufferTag {};
}

using DrawShader = SafeHandle<std::uint16_t, priv::DrawShaderTag>;
using DrawTexture = SafeHandle<std::uint32_t, priv::DrawTextureTag>;
using DrawFramebuffer = SafeHandle<std::uint16_t, priv::DrawFramebufferTag>;

}

#endif /* LUMS_INCLUDED_DRAW_TYPES_H */

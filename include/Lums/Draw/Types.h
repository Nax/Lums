#ifndef LUMS_INCLUDED_DRAW_TYPES_H
#define LUMS_INCLUDED_DRAW_TYPES_H

#include <cstdint>
#include <Lums/Util/SafeHandle.h>

namespace lm
{

namespace priv
{
    struct DrawFramebufferTag {};
    struct DrawTextureTag {};
}

using DrawFramebuffer = SafeHandle<std::uint16_t, priv::DrawFramebufferTag>;
using DrawTexture = SafeHandle<std::uint32_t, priv::DrawTextureTag>;

}

#endif /* LUMS_INCLUDED_DRAW_TYPES_H */

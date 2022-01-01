#include <Lums/Draw/DrawRendererVulkan.h>

using namespace lm;

namespace
{

void dummyHandler(void*, priv::DrawCommand* cmd)
{

}

}

DrawRendererVulkan::DrawRendererVulkan(Window& win)
: DrawRenderer{win}
{
    for (int i = 0; i < 32; ++i)
        _handlers[i] = (lm::priv::DrawCommandHandler)&dummyHandler;
}

DrawRendererVulkan::~DrawRendererVulkan()
{

}

bool DrawRendererVulkan::valid() const
{
    return true;
}


void DrawRendererVulkan::swap()
{

}

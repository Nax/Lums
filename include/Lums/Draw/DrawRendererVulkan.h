#ifndef LUMS_INCLUDED_DRAW_RENDERER_VULKAN_H
#define LUMS_INCLUDED_DRAW_RENDERER_VULKAN_H

#include <vulkan/vulkan.h>
#include <Lums/Draw/DrawRenderer.h>

namespace lm
{

class DrawRendererVulkan : public DrawRenderer
{
public:
    LUMS_API_DRAW DrawRendererVulkan(Window& win);
    LUMS_API_DRAW ~DrawRendererVulkan();

    LUMS_API_DRAW bool valid() const override;

protected:
    LUMS_API_DRAW void swap() override;

private:
    VkInstance _instance;
};

}

#endif /* LUMS_INCLUDED_DRAW_RENDERER_VULKAN_H */

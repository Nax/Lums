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
    void initInstance();
    void initPhysicalDevice();
    void initDevice();
    void initSwapchain();
    void initSwapchainImageViews();

    void platformAppendInstanceExtensions(std::vector<const char*>& exts);
    void platformInitSurface();

    VkInstance                  _instance;
    VkPhysicalDevice            _physicalDevice;
    VkDevice                    _device;
    VkQueue                     _queueGraphics;
    VkQueue                     _queuePresent;
    VkSurfaceKHR                _surface;
    VkSwapchainKHR              _swapchain;
    VkFormat                    _swapchainFormat;
    VkExtent2D                  _swapchainExtent;
    std::vector<VkImage>        _swapchainImages;
    std::vector<VkImageView>    _swapchainImageViews;
};

}

#endif /* LUMS_INCLUDED_DRAW_RENDERER_VULKAN_H */

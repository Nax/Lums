#define VK_USE_PLATFORM_WIN32_KHR

#include <Lums/Draw/DrawRendererVulkan.h>
#include <Lums/Window/Window.h>

using namespace lm;

void DrawRendererVulkan::platformAppendInstanceExtensions(std::vector<const char*>& exts)
{
    exts.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
}

void DrawRendererVulkan::platformInitSurface()
{
    VkWin32SurfaceCreateInfoKHR info{};
    info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    info.hwnd = _window.hwnd();
    info.hinstance = GetModuleHandle(nullptr);

    vkCreateWin32SurfaceKHR(_instance, &info, nullptr, &_surface);
}

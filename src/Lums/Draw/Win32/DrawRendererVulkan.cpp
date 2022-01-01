#define VK_USE_PLATFORM_WIN32_KHR

#include <Lums/Draw/DrawRendererVulkan.h>
#include <Lums/Window/Window.h>

using namespace lm;

void DrawRendererVulkan::platformInitSurface()
{
    VkWin32SurfaceCreateInfoKHR info{};
    info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    info.hwnd = _window.hwnd();
    info.hinstance = GetModuleHandle(nullptr);

    vkCreateWin32SurfaceKHR(_instance, &info, nullptr, &_surface);
}

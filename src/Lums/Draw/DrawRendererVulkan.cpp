#include <algorithm>
#include <set>
#include <Lums/Draw/DrawRendererVulkan.h>
#include <Lums/Window/Window.h>
#include <Lums/Math/Algebra.h>

using namespace lm;

namespace
{

struct QueueFamilyIndices
{
    std::uint32_t   graphics;
    std::uint32_t   present;
};

struct SwapchainInfos
{
    VkSurfaceCapabilitiesKHR caps;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

void dummyHandler(void*, priv::DrawCommand* cmd)
{

}

bool getQueueFamilyIndices(VkPhysicalDevice device, VkSurfaceKHR surface, QueueFamilyIndices& queues)
{
    std::uint32_t queueFamilyCount{};
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies{queueFamilyCount};
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    bool done{};
    bool graphics{};
    bool present{};
    VkBool32 tmp;

    for (int i = 0; i < (int)queueFamilies.size(); ++i)
    {
        const auto& qf = queueFamilies[i];
        if (qf.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            queues.graphics = i;
            graphics = true;
        }

        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &tmp);
        if (tmp)
        {
            queues.present = i;
            present = true;
        }

        done = (graphics && present);
        if (done)
            break;
    }

    return done;
}

void getSwapchainInfos(VkPhysicalDevice device, VkSurfaceKHR surface, SwapchainInfos& infos)
{
    std::uint32_t count{};

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &infos.caps);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr);
    infos.formats.resize(count);
    if (count)
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, infos.formats.data());
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, nullptr);
    infos.presentModes.resize(count);
    if (count)
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, infos.presentModes.data());
}

bool checkPhysicalDeviceExtensionSupport(VkPhysicalDevice device, const char* requiredExt)
{
    uint32_t count;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);

    std::vector<VkExtensionProperties> extensions(count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &count, extensions.data());

    for (const auto& e : extensions)
    {
        if (std::strcmp(e.extensionName, requiredExt) == 0)
            return true;
    }
    return false;
}

bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices dummy;
    SwapchainInfos swapchainInfos;

    if (!checkPhysicalDeviceExtensionSupport(device, VK_KHR_SWAPCHAIN_EXTENSION_NAME))
        return false;
    if (!getQueueFamilyIndices(device, surface, dummy))
        return false;
    getSwapchainInfos(device, surface, swapchainInfos);
    if (swapchainInfos.formats.empty() || swapchainInfos.presentModes.empty())
        return false;
    return true;
}

VkSurfaceFormatKHR getBestFormat(const std::vector<VkSurfaceFormatKHR>& formats)
{
    for (const auto& f : formats)
    {
        if (f.format == VK_FORMAT_B8G8R8A8_SRGB && f.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
            return f;
    }
    return formats[0];
}

VkPresentModeKHR getBestPresentMode(const std::vector<VkPresentModeKHR>& presentModes)
{
    (void)presentModes;
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D getSwapExtent(const VkSurfaceCapabilitiesKHR& caps, Vector2i winSize)
{
    if (caps.currentExtent.width != UINT32_MAX)
        return caps.currentExtent;

    VkExtent2D ex;
    ex.width = clamp((std::uint32_t)winSize.x, caps.minImageExtent.width, caps.maxImageExtent.width);
    ex.height = clamp((std::uint32_t)winSize.y, caps.minImageExtent.height, caps.maxImageExtent.height);
    return ex;
}

}

DrawRendererVulkan::DrawRendererVulkan(Window& win)
: DrawRenderer{win}
, _instance{}
, _physicalDevice{}
, _device{}
, _queueGraphics{}
, _surface{}
, _swapchain{}
, _swapchainFormat{}
, _swapchainExtent{}
{
    /* Dummy */
    for (int i = 0; i < 32; ++i)
        _handlers[i] = (lm::priv::DrawCommandHandler)&dummyHandler;

    initInstance();
    platformInitSurface();
    initPhysicalDevice();
    initDevice();
    initSwapchain();
    initSwapchainImageViews();
}

DrawRendererVulkan::~DrawRendererVulkan()
{
    if (_instance == VK_NULL_HANDLE)
        return;
    vkDestroyInstance(_instance, nullptr);
}

bool DrawRendererVulkan::valid() const
{
    return true;
}


void DrawRendererVulkan::swap()
{
}

void DrawRendererVulkan::initInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "???";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Lums";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char*> layers;
#if defined(DEBUG)
    layers.push_back("VK_LAYER_KHRONOS_validation");
#endif

    std::vector<const char*> extensions;
    extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    platformAppendInstanceExtensions(extensions);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledLayerCount = (std::uint32_t)layers.size();
    createInfo.ppEnabledLayerNames = layers.data();
    createInfo.enabledExtensionCount = (std::uint32_t)extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    vkCreateInstance(&createInfo, nullptr, &_instance);
}

void DrawRendererVulkan::initPhysicalDevice()
{
    std::uint32_t deviceCount{};
    std::vector<VkPhysicalDevice> candidates;

    /* Load the devices */
    vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);
    candidates.resize(deviceCount);
    vkEnumeratePhysicalDevices(_instance, &deviceCount, candidates.data());

    /* Get only the suitable ones */
    auto res = std::remove_if(candidates.begin(), candidates.end(), [this](VkPhysicalDevice device) { return !isDeviceSuitable(device, _surface); });
    candidates.erase(res, candidates.end());

    /* Get the device */
    _physicalDevice = candidates[0];
}

void DrawRendererVulkan::initDevice()
{
    std::set<std::uint32_t> uniqueFamilies;
    float priority = 1.f;

    QueueFamilyIndices queueFamilyIndices;
    getQueueFamilyIndices(_physicalDevice, _surface, queueFamilyIndices);
    uniqueFamilies.insert(queueFamilyIndices.graphics);
    uniqueFamilies.insert(queueFamilyIndices.present);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    for (std::uint32_t family : uniqueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = family;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &priority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    std::vector<const char*> extensions;
    extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = (std::uint32_t)queueCreateInfos.size();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledExtensionCount = (std::uint32_t)extensions.size();

    /* Create the logical device */
    vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device);

    /* Fetch the queue handles for future use */
    vkGetDeviceQueue(_device, queueFamilyIndices.graphics, 0, &_queueGraphics);
    vkGetDeviceQueue(_device, queueFamilyIndices.present, 0, &_queuePresent);
}

void DrawRendererVulkan::initSwapchain()
{
    SwapchainInfos swapchainInfos{};
    getSwapchainInfos(_physicalDevice, _surface, swapchainInfos);

    QueueFamilyIndices indices{};
    getQueueFamilyIndices(_physicalDevice, _surface, indices);

    VkSurfaceFormatKHR format = getBestFormat(swapchainInfos.formats);
    VkPresentModeKHR presentMode = getBestPresentMode(swapchainInfos.presentModes);
    VkExtent2D extent = getSwapExtent(swapchainInfos.caps, _window.size());

    std::uint32_t imageCount = swapchainInfos.caps.minImageCount + 1;
    if (swapchainInfos.caps.maxImageCount > 0 && swapchainInfos.caps.maxImageCount < imageCount)
        imageCount = swapchainInfos.caps.maxImageCount;

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = _surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = format.format;
    createInfo.imageColorSpace = format.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    std::vector<std::uint32_t> boundQueues;
    if (indices.graphics == indices.present)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        boundQueues.push_back(indices.graphics);
        boundQueues.push_back(indices.present);
    }

    createInfo.queueFamilyIndexCount = (std::uint32_t)boundQueues.size();
    if (!boundQueues.empty())
        createInfo.pQueueFamilyIndices = boundQueues.data();

    createInfo.preTransform = swapchainInfos.caps.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapchain);

    std::uint32_t imgCount{};
    vkGetSwapchainImagesKHR(_device, _swapchain, &imgCount, nullptr);
    _swapchainImages.resize(imgCount);
    vkGetSwapchainImagesKHR(_device, _swapchain, &imgCount, _swapchainImages.data());

    _swapchainFormat = format.format;
    _swapchainExtent = extent;
}

void DrawRendererVulkan::initSwapchainImageViews()
{
    _swapchainImageViews.resize(_swapchainImages.size());
    for (std::size_t i = 0; i < _swapchainImages.size(); ++i)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = _swapchainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = _swapchainFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        vkCreateImageView(_device, &createInfo, nullptr, _swapchainImageViews.data() + i);
    }
}

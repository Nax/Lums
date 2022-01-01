#include <set>
#include <Lums/Draw/DrawRendererVulkan.h>

using namespace lm;

namespace
{

struct QueueFamilyIndices
{
    std::uint32_t   graphics;
    std::uint32_t   present;
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

bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices dummy;

    if (!getQueueFamilyIndices(device, surface, dummy))
        return false;
    return true;
}

}

DrawRendererVulkan::DrawRendererVulkan(Window& win)
: DrawRenderer{win}
, _instance{VK_NULL_HANDLE}
, _physicalDevice{VK_NULL_HANDLE}
, _device{VK_NULL_HANDLE}
, _queueGraphics{VK_NULL_HANDLE}
, _surface{VK_NULL_HANDLE}
{
    /* Dummy */
    for (int i = 0; i < 32; ++i)
        _handlers[i] = (lm::priv::DrawCommandHandler)&dummyHandler;

    initInstance();
    initPhysicalDevice();
    platformInitSurface();
    initDevice();
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

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

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
    auto res = std::remove_if(candidates.begin(), candidates.end(), [this](VkPhysicalDevice device) { return isDeviceSuitable(device, _surface); });
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

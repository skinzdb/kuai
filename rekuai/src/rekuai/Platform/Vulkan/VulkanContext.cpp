#include "kpch.h"

#include "VulkanContext.h"
#include "rekuai/Platform/Vulkan/VulkanUtils.h"

#include <set>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace kuai {

    const std::vector<const char*> validation_layers = {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> device_exts = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_EXT_SHADER_OBJECT_EXTENSION_NAME,
        VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME
    };

    #ifdef NDEBUG
        const bool enable_validation = false;
    #else
        const bool enable_validation = true;
    #endif

    VulkanContext::VulkanContext(const std::string& app_name, const std::string& engine_name)
        : app_name(app_name), engine_name(engine_name)
    {
        create_instance();
        setup_debug_messenger();
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {

        switch(messageSeverity)
        {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            KU_CORE_INFO("(Validation layer) {}", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            KU_CORE_INFO("(Validation layer) {}", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            KU_CORE_WARN("(Validation layer) {}", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            KU_CORE_ERROR("(Validation layer) {}", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
            KU_CORE_CRITICAL("(Validation layer) {}", pCallbackData->pMessage);
            break;
        }

        return VK_FALSE;
    }

    VkResult create_debug_utils_messenger_ext(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void destroy_debug_utils_messenger_ext(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger, const VkAllocationCallbacks* pAllocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debug_messenger, pAllocator);
        }
    }

    void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debug_callback;
    }

    void VulkanContext::cleanup_devices()
    {
        vkDestroyDevice(device, nullptr);

        if (enable_validation)
        {
            destroy_debug_utils_messenger_ext(instance, debug_messenger, nullptr);
        }
    }

    void VulkanContext::cleanup_instance()
    {
        vkDestroyInstance(instance, nullptr);
    }

    void VulkanContext::setup_debug_messenger()
    {
        if (!enable_validation) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        populate_debug_messenger_create_info(createInfo);

        if (create_debug_utils_messenger_ext(instance, &createInfo, nullptr, &debug_messenger) != VK_SUCCESS) {
            KU_CORE_CRITICAL("(Vulkan) Failed to set up debug messenger");
            exit(1);
        }
    }

    bool check_validation_layer_support()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validation_layers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    std::vector<const char*> get_required_extensions()
    {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enable_validation) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    void VulkanContext::create_instance()
    {
        if (enable_validation && !check_validation_layer_support())
        {
            KU_CORE_CRITICAL("(Vulkan) Validation layers requested, but not available");
            exit(1);
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = app_name.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = engine_name.c_str();
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions = get_required_extensions();

        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (enable_validation)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
            createInfo.ppEnabledLayerNames = validation_layers.data();

            populate_debug_messenger_create_info(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
        }
        else
        {
            createInfo.enabledLayerCount = 0;

            createInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        {
            KU_CORE_CRITICAL("Failed to create Vulkan instance");
            exit(1);
        }
    }

    bool check_device_extension_support(VkPhysicalDevice device)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(device_exts.begin(), device_exts.end());

        for (const auto& extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }

        for (const auto& extension : requiredExtensions)
        {
            KU_CORE_WARN("(Vulkan) {} extension not supported", extension);
        }

        return requiredExtensions.empty();
    }

    bool is_device_suitable(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        QueueFamilyIndices indices = VulkanUtils::find_queue_families(device, surface);

        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensions.data());

        bool extensionsSupported = check_device_extension_support(device);

        bool swapChainAdequate = false;
        if (extensionsSupported)
        {
            SwapChainSupportDetails swapChainSupport = VulkanUtils::query_swap_chain_support(device, surface);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.present_modes.empty();
        }

        return indices.is_ok() && extensionsSupported && swapChainAdequate;
    }

    void VulkanContext::pick_physical_device(VkSurfaceKHR surface)
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            KU_CORE_CRITICAL("(Vulkan) Failed to find GPU device with Vulkan support");
            exit(1);
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (const auto& device : devices)
        {
            VkPhysicalDeviceProperties deviceProps{};
            vkGetPhysicalDeviceProperties(device, &deviceProps);

            
            KU_CORE_INFO("(Vulkan) Checking GPU device {}, API version: {}.{}.{}", 
                deviceProps.deviceName, 
                VK_API_VERSION_MAJOR(deviceProps.apiVersion),
                VK_API_VERSION_MINOR(deviceProps.apiVersion),
                VK_API_VERSION_PATCH(deviceProps.apiVersion)
            );

            if (is_device_suitable(device, surface))
            {
                physical_device = device;
                break;
            }
        }

        if (physical_device == VK_NULL_HANDLE)
        {
            KU_CORE_CRITICAL("(Vulkan) Failed to find suitable GPU device");
            exit(1);
        }
    }

    void VulkanContext::create_logical_device(VkSurfaceKHR surface)
    {
        QueueFamilyIndices indices = VulkanUtils::find_queue_families(physical_device, surface);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {
            indices.graphics_family.value(),
            indices.present_family.value()
        };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        VkPhysicalDeviceFeatures deviceFeatures{};
        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount   = static_cast<uint32_t>(device_exts.size());
        createInfo.ppEnabledExtensionNames = device_exts.data();

        // Add shader object extension feature
        VkPhysicalDeviceShaderObjectFeaturesEXT shaderFeatures{};
        shaderFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_OBJECT_FEATURES_EXT;
        shaderFeatures.shaderObject = true;
        createInfo.pNext = &shaderFeatures;

        // Add dynamic rendering extension feature
        VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicFeatures{};
        dynamicFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
        dynamicFeatures.dynamicRendering = true;
        shaderFeatures.pNext = &dynamicFeatures;

        // Add device descriptor indexing feature
        VkPhysicalDeviceDescriptorIndexingFeatures descriptorFeatures{};
        descriptorFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
        descriptorFeatures.descriptorBindingPartiallyBound = true;
        descriptorFeatures.runtimeDescriptorArray = true;
        dynamicFeatures.pNext = &descriptorFeatures;

        if (enable_validation)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
            createInfo.ppEnabledLayerNames = validation_layers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(physical_device, &createInfo, nullptr, &device) != VK_SUCCESS)
        {
            KU_CORE_CRITICAL("(Vulkan) Failed to create logical device");
            exit(1);
        }

        vkGetDeviceQueue(device, indices.graphics_family.value(), 0, &graphics_queue);
        vkGetDeviceQueue(device, indices.present_family.value(), 0, &present_queue);
    }
}

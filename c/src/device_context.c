#ifndef ILY_DEVICE_CONTEXT
#define ILY_DEVICE_CONTEXT
#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan_core.h>

#include "context.h"
#include "ily_types.h"

static const char* requiredExtensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
const size_t reqExtensionCount = 1;

size_t isDeviceSuitable(AppContext* appContext, VkPhysicalDevice device);
size_t checkDeviceExtensionSupport(VkPhysicalDevice device);

void pickPhysicalDevice(AppContext* appContext) {
    uint32_t physicalDeviceCount;
    vkEnumeratePhysicalDevices(appContext->instance, &physicalDeviceCount, nullptr);
    VkPhysicalDevice physicalDevices[physicalDeviceCount];

    if (physicalDeviceCount == 0) {
        appContext->fp_errBack(ILY_FAILED_TO_FIND_GPU_WITH_VULKAN_SUPPORT);
    }

    if (vkEnumeratePhysicalDevices(appContext->instance, &physicalDeviceCount, physicalDevices) != VK_SUCCESS) {
        appContext->fp_errBack(ILY_FAILED_TO_FIND_GPU_WITH_VULKAN_SUPPORT);
    };

    for (size_t i = 0; i < physicalDeviceCount; ++i) {
        VkPhysicalDevice targetDevice = physicalDevices[i];
        if (isDeviceSuitable(appContext, targetDevice)) {
            appContext->physicalDevice = targetDevice;
            break;
        }
    }

    if (appContext->physicalDevice == VK_NULL_HANDLE) {
        appContext->fp_errBack(ILY_FAILED_TO_FIND_GPU_WITH_VULKAN_SUPPORT);
    }

    // for debug
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(appContext->physicalDevice, &deviceProperties);
    printf("Device Name: %s\n", deviceProperties.deviceName);
}

size_t checkDeviceExtensionSupport(VkPhysicalDevice device, const char* requiredExtension) {
    uint32_t deviceExtensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &deviceExtensionCount, nullptr);
    VkExtensionProperties deviceExtensions[deviceExtensionCount];
    vkEnumerateDeviceExtensionProperties(device, nullptr, &deviceExtensionCount, deviceExtensions);
    for (int i = 0; i < deviceExtensionCount; ++i) {
        if (strcmp(deviceExtensions[i].extensionName, requiredExtension) == 0) {
            return 1;
        }
    }
    return 0;
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(vector* availableFormats) {
    for (size_t i = 0; i < availableFormats->count; ++i) {
        VkSurfaceFormatKHR* format = (VkSurfaceFormatKHR*)vector_get(availableFormats, i);
        if (format->format == VK_FORMAT_B8G8R8_SRGB && format->colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return *format;
        }
    }

    VkSurfaceFormatKHR* format = (VkSurfaceFormatKHR*)vector_get(availableFormats, 0);
    return *format;
}

VkPresentModeKHR chooseSwapPresentMode(vector* availablePresentModes) {
    for (size_t i = 0; i < availablePresentModes->count; ++i) {
        VkPresentModeKHR presentMode = *(VkPresentModeKHR*)vector_get(availablePresentModes, i);
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return presentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(AppContext* appContext, const VkSurfaceCapabilitiesKHR* capabilities) {
    if (capabilities->currentExtent.width != UINT32_MAX) {
        return capabilities->currentExtent;
    } else {
        int width, height = 0;
        int c_width = capabilities->currentExtent.width;
        int c_height = capabilities->currentExtent.height;
        glfwGetFramebufferSize(appContext->window, &width, &height);

        VkExtent2D actualExtent = {
          (uint32_t)((width < c_width) ? width : c_width),
          (uint32_t)((height < c_height) ? height : c_height)};

        return actualExtent;
    }
}

SwapChainSupportDetails querySwapChainSupport(AppContext* appContext, VkPhysicalDevice device) {
    SwapChainSupportDetails details;
    vector_init(&details.formats, sizeof(VkSurfaceFormatKHR*));
    vector_init(&details.presentModes, sizeof(VkPresentModeKHR*));

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, appContext->surface, &details.capabilities);
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, appContext->surface, &formatCount, nullptr);
    if (formatCount != 0) {
        VkSurfaceFormatKHR deviceFormats[formatCount];
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, appContext->surface, &formatCount, deviceFormats);
        for (size_t i = 0; i < formatCount; ++i) {
            vector_append(&details.formats, &deviceFormats[i]);
        }
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, appContext->surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        VkPresentModeKHR presentModes[presentModeCount];
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, appContext->surface, &presentModeCount, presentModes);
        for (size_t i = 0; i < presentModeCount; ++i) {
            vector_append(&details.presentModes, &presentModes[i]);
        }
    }

    return details;
}

size_t isDeviceSuitable(AppContext* appContext, VkPhysicalDevice device) {
    QueueFamilyIndices indices = findQueueFamilies(appContext, device);

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    for (int i = 0; i < reqExtensionCount; ++i) {
        int swapChainAdequate = 0;
        int extensionSupported = checkDeviceExtensionSupport(device, requiredExtensions[i]);
        if (extensionSupported) {
            SwapChainSupportDetails details = querySwapChainSupport(appContext, device);
            swapChainAdequate = !vector_empty(&details.formats) && !vector_empty(&details.presentModes);
        }
        if (indices.graphicsFamily.isPresent && deviceFeatures.geometryShader && extensionSupported && swapChainAdequate) {
            return 1;
        }
    }
    return 0;
}

QueueFamilyIndices findQueueFamilies(AppContext* appContext, VkPhysicalDevice device) {
    QueueFamilyIndices indices = {};
    uint32_t queueFamilyCount;

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    VkQueueFamilyProperties queueFamilies[queueFamilyCount];

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    for (size_t i = 0; i < queueFamilyCount; ++i) {
        VkQueueFamilyProperties targetFamily = queueFamilies[i];
        if (targetFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            option some1 = option_wrap(&i, sizeof(i));
            indices.graphicsFamily = some1;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, appContext->surface, &presentSupport);
        if (presentSupport) {
            option some2 = option_wrap(&i, sizeof(i));
            indices.presentFamily = some2;
        }

        if (indices.graphicsFamily.isPresent && indices.presentFamily.isPresent) {
            break;
        }
    }
    return indices;
}

void createLogicalDevice(AppContext* appContext) {
    QueueFamilyIndices indices = findQueueFamilies(appContext, appContext->physicalDevice);

    VkDeviceQueueCreateInfo queueCreateInfos[2]{};
    VkPhysicalDeviceFeatures deviceFeatures{};
    VkDeviceCreateInfo createInfo{};
    float queuePriority = 1.0f;

    // Check for the presence of required queue families
    if (!indices.graphicsFamily.isPresent) {
        appContext->fp_errBack(ILY_GRAPHICS_FAMILY_NOT_PRESENT);
        return;
    }
    if (!indices.presentFamily.isPresent) {
        appContext->fp_errBack(ILY_PRESENT_FAMILY_NOT_PRESENT);
        return;
    }

    uint32_t* pGraphicsFamily = (uint32_t*)option_unwrap(&indices.graphicsFamily);
    uint32_t* pPresentFamily = (uint32_t*)option_unwrap(&indices.presentFamily);
    uint32_t graphicsFamilyIndex = *pGraphicsFamily;
    uint32_t presentFamilyIndex = *pPresentFamily;

    createInfo.queueCreateInfoCount = 1;
    // Graphics queue creation info
    queueCreateInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfos[0].queueFamilyIndex = graphicsFamilyIndex;
    queueCreateInfos[0].queueCount = 1;
    queueCreateInfos[0].pQueuePriorities = &queuePriority;

    if (presentFamilyIndex != graphicsFamilyIndex) {
        // Present queue creation info
        queueCreateInfos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfos[1].queueFamilyIndex = presentFamilyIndex;
        queueCreateInfos[1].queueCount = 1;
        queueCreateInfos[1].pQueuePriorities = &queuePriority;
        createInfo.queueCreateInfoCount = 2;
    }

    // Device creation info
    createInfo.pQueueCreateInfos = queueCreateInfos;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    // Swapchain enabling
    // once again just hardcoding this for the sake of completing the tutorial
    createInfo.enabledExtensionCount = reqExtensionCount;
    createInfo.ppEnabledExtensionNames = requiredExtensions;

    // Create the logical devic
    if (vkCreateDevice(appContext->physicalDevice, &createInfo, nullptr, &appContext->logicalDevice) != VK_SUCCESS) {
        appContext->fp_errBack(ILY_LOGICAL_DEVICE_CREATION_FAILED);
        return;
    }

    // Get the device queues
    vkGetDeviceQueue(appContext->logicalDevice, graphicsFamilyIndex, 0, &appContext->graphicsQueue);
    vkGetDeviceQueue(appContext->logicalDevice, presentFamilyIndex, 0, &appContext->presentQueue);
}

void createSwapchain(AppContext* appContext) {
    SwapChainSupportDetails details = querySwapChainSupport(appContext, appContext->physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(&details.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(&details.presentModes);
    VkExtent2D extent2d = chooseSwapExtent(appContext, &details.capabilities);
    uint32_t imageCount = details.capabilities.minImageCount + 1;

    if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount) {
        imageCount = details.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = appContext->surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent2d;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(appContext, appContext->physicalDevice);
    uint32_t graphicsFamily = *(uint32_t*)option_unwrap(&indices.graphicsFamily);
    uint32_t presentFamily = *(uint32_t*)option_unwrap(&indices.presentFamily);
    uint32_t queueFamilyIndices[] = {graphicsFamily, presentFamily};

    if (graphicsFamily != presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;     // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }
    createInfo.preTransform = details.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(appContext->logicalDevice, &createInfo, nullptr, &appContext->swapchainContext.swapchain) != VK_SUCCESS) {
        appContext->fp_errBack(ILY_FAILED_TO_CREATE_SWAPCHAIN);
    }
    vkGetSwapchainImagesKHR(appContext->logicalDevice, appContext->swapchainContext.swapchain, &imageCount, nullptr);
    VkImage images[imageCount];
    vector_init(&appContext->swapchainContext.swapchainImages, sizeof(VkImage*));
    vkGetSwapchainImagesKHR(appContext->logicalDevice, appContext->swapchainContext.swapchain, &imageCount, images);
    for (size_t i = 0; i < imageCount; ++i) {
        vector_append(&appContext->swapchainContext.swapchainImages, &images[i]);
    }

    appContext->swapchainContext.swapchainImageFormat = surfaceFormat.format;
    appContext->swapchainContext.swapchainExtent = extent2d;


}

#endif /* ifndef ILY_DEVICE_CONTEXT */

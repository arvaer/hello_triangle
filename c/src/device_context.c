#ifndef ILY_DEVICE_CONTEXT
#define ILY_DEVICE_CONTEXT
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

size_t isDeviceSuitable(AppContext* appContext, VkPhysicalDevice device) {
    QueueFamilyIndices indices = findQueueFamilies(appContext, device);

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    for (int i = 0; i < reqExtensionCount; ++i) {
        if (indices.graphicsFamily.isPresent && deviceFeatures.geometryShader && checkDeviceExtensionSupport(device, requiredExtensions[i])) {
            return 1;
        }
    }
    return 0;
}

QueueFamilyIndices findQueueFamilies(AppContext* appContext, VkPhysicalDevice device) {
    QueueFamilyIndices indices = {};
    uint32_t queueFamilyCount;
    VkBool32 presentSupport = false;

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    VkQueueFamilyProperties queueFamilies[queueFamilyCount];

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    for (size_t i = 0; i < queueFamilyCount; ++i) {
        VkQueueFamilyProperties targetFamily = queueFamilies[i];
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, appContext->surface, &presentSupport);
        if (targetFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            option some = option_wrap(&i, sizeof(i));;
            indices.graphicsFamily = some;
        }

        if (presentSupport) {
            option some = option_wrap(&i, sizeof(i));;
            indices.presentFamily = some;
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

    // Graphics queue creation info
    queueCreateInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfos[0].queueFamilyIndex = graphicsFamilyIndex;
    queueCreateInfos[0].queueCount = 1;
    queueCreateInfos[0].pQueuePriorities = &queuePriority;

    // Present queue creation info
    queueCreateInfos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfos[1].queueFamilyIndex = presentFamilyIndex;
    queueCreateInfos[1].queueCount = 1;
    queueCreateInfos[1].pQueuePriorities = &queuePriority;

    // Device creation info
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 2;
    createInfo.pQueueCreateInfos = queueCreateInfos;
    createInfo.pEnabledFeatures = &deviceFeatures;

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

#endif /* ifndef ILY_DEVICE_CONTEXT */

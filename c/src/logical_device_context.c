#ifndef ILY_LOGICAL_DEVICE_CONTEXT
#define ILY_LOGICAL_DEVICE_CONTEXT
#include "context.h"
#include <cstdlib>

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

    // Graphics queue creation info
    queueCreateInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfos[0].queueFamilyIndex = indices.graphicsFamily.value;
    queueCreateInfos[0].queueCount = 1;
    queueCreateInfos[0].pQueuePriorities = &queuePriority;

    // Present queue creation info
    queueCreateInfos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfos[1].queueFamilyIndex = indices.presentFamily.value;
    queueCreateInfos[1].queueCount = 1;
    queueCreateInfos[1].pQueuePriorities = &queuePriority;

    // Device creation info
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 2;
    createInfo.pQueueCreateInfos = queueCreateInfos;
    createInfo.pEnabledFeatures = &deviceFeatures;

    // Swapchain enabling
    // once again just hardcoding this for the sake of completing the tutorial
    ExtensionNames extensions = {.count = 1, .capacity = 5};
    extensions.items = (const char**)malloc(extensions.count * sizeof(const char**));
    extensions.items[0] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    createInfo.enabledExtensionCount = extensions.count;
    createInfo.ppEnabledExtensionNames = extensions.items;

    // Create the logical device
    if (vkCreateDevice(appContext->physicalDevice, &createInfo, nullptr, &appContext->logicalDevice) != VK_SUCCESS) {
        appContext->fp_errBack(ILY_LOGICAL_DEVICE_CREATION_FAILED);
        return;
    }

    // Get the device queues
    vkGetDeviceQueue(appContext->logicalDevice, indices.graphicsFamily.value, 0, &appContext->graphicsQueue);
    vkGetDeviceQueue(appContext->logicalDevice, indices.presentFamily.value, 0, &appContext->presentQueue);
}


#endif /* ifndef ILY_LOGICAL_DEVICE_CONTEXT */

#ifndef ILY_LOGICAL_DEVICE_CONTEXT
#define ILY_LOGICAL_DEVICE_CONTEXT
#include "context.h"
#include <vulkan/vulkan_core.h>

void createLogicalDevice(AppContext* appContext) {
    QueueFamilyIndices indices = findQueueFamilies(appContext->physicalDevice);
    VkDeviceQueueCreateInfo queueCreateInfo{};
    // leaving empty until we're ready to use in later chapters
    VkPhysicalDeviceFeatures deviceFeatures{};
    VkDeviceCreateInfo createInfo{};

    // QueueCreationLogic
    float queuePriority = 1.0f;
    if (!indices.graphicsFamily.isPresent) {
        appContext->fp_errBack(ILY_GRAPHICS_FAMILY_NOT_PRESENT);
    }

    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    // DeviceFeature Logic

    // Logical Device Creatioon Logic
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;

    if (vkCreateDevice(appContext->physicalDevice, &createInfo, nullptr, &appContext->logicalDevice) != VK_SUCCESS) {
        appContext->fp_errBack(ILY_LOGICAL_DEVICE_CREATION_FAILED);
    }

    vkGetDeviceQueue(appContext->logicalDevice, indices.graphicsFamily.value, 0, &appContext->graphicsQueue);
};

#endif /* ifndef ILY_LOGICAL_DEVICE_CONTEXT */

#ifndef ILY_DEVICE_CONTEXT
#define ILY_DEVICE_CONTEXT
#include "context.h"
#include "ily_errors.h"
#include "ily_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

typedef struct {
    VkPhysicalDevice* items;
    uint32_t count;
    uint32_t capacity;
} PhysicalDevices;

typedef struct {
    VkQueueFamilyProperties* items;
    uint32_t count;
    uint32_t capacity;
} QueueFamilies;

struct QueueFamilyIndices {
    opt_uint32_t graphicsFamily = {};
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
int isDeviceSuitable(VkPhysicalDevice device);

void pickPhysicalDevice(AppContext* appContext) {
    PhysicalDevices physicalDevices = {};
    vkEnumeratePhysicalDevices(appContext->instance, &physicalDevices.count,
                               nullptr);
    physicalDevices.capacity = physicalDevices.count * 2;
    physicalDevices.items = (VkPhysicalDevice*)malloc(
      physicalDevices.count * sizeof(VkPhysicalDevice*));

    if (physicalDevices.count == 0) {
        appContext->fp_errBack(ILY_FAILED_TO_FIND_GPU_WITH_VULKAN_SUPPORT);
    }

    if (vkEnumeratePhysicalDevices(appContext->instance, &physicalDevices.count, physicalDevices.items) != VK_SUCCESS) {
        appContext->fp_errBack(ILY_FAILED_TO_FIND_GPU_WITH_VULKAN_SUPPORT);
    };

    for (size_t i = 0; i < physicalDevices.count; ++i) {
        VkPhysicalDevice targetDevice = physicalDevices.items[i];
        if (isDeviceSuitable(targetDevice)) {
            appContext->physicalDevice = targetDevice;
            break;
        }
    }

    if (appContext->physicalDevice == VK_NULL_HANDLE) {
        appContext->fp_errBack(ILY_FAILED_TO_FIND_GPU_WITH_VULKAN_SUPPORT);
    }

    free(physicalDevices.items);

    // for debug
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(appContext->physicalDevice, &deviceProperties);
    printf("Device Name: %s\n", deviceProperties.deviceName);
}

int isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = findQueueFamilies(device);

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    if (indices.graphicsFamily.isPresent && deviceFeatures.geometryShader) {
        return 1;
    }

    return 0;
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilies queuefamilies = {};
    QueueFamilyIndices indices = {.graphicsFamily = {.isPresent = 0, .value = 0}};

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queuefamilies.count, nullptr);
    queuefamilies.capacity = queuefamilies.count * 2;
    queuefamilies.items = (VkQueueFamilyProperties*)malloc(queuefamilies.count * sizeof(VkQueueFamilyProperties));

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queuefamilies.count, queuefamilies.items);
    for (size_t i = 0; i < queuefamilies.count; ++i) {
        VkQueueFamilyProperties targetFamily = queuefamilies.items[i];
        if (targetFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            opt_uint32_t some = {.isPresent = 1, .value = (uint32_t)i};
            indices.graphicsFamily = some;
        }
    }
    return indices;
}

#endif /* ifndef ILY_DEVICE_CONTEXT */

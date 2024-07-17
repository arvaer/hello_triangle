#ifndef ILY_PHYSICAL_DEVICE_CONTEXT
#define ILY_PHYSICAL_DEVICE_CONTEXT
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

#include "context.h"
#include "ily_types.h"

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

typedef struct {
    VkExtensionProperties* items;
    uint32_t count;
    uint32_t capacity;
} Extensions;


size_t isDeviceSuitable(AppContext* appContext, VkPhysicalDevice device);
size_t checkDeviceExtensionSupport(VkPhysicalDevice device);

void pickPhysicalDevice(AppContext* appContext) {
    PhysicalDevices physicalDevices = {};
    vkEnumeratePhysicalDevices(appContext->instance, &physicalDevices.count, nullptr);


    physicalDevices.capacity = physicalDevices.count * 2;
    physicalDevices.items = (VkPhysicalDevice*)malloc(physicalDevices.count * sizeof(VkPhysicalDevice));

    if (physicalDevices.count == 0) {
        appContext->fp_errBack(ILY_FAILED_TO_FIND_GPU_WITH_VULKAN_SUPPORT);
    }

    if (vkEnumeratePhysicalDevices(appContext->instance, &physicalDevices.count, physicalDevices.items) != VK_SUCCESS) {
        appContext->fp_errBack(ILY_FAILED_TO_FIND_GPU_WITH_VULKAN_SUPPORT);
    };

    for (size_t i = 0; i < physicalDevices.count; ++i) {
        VkPhysicalDevice targetDevice = physicalDevices.items[i];
        if (isDeviceSuitable(appContext, targetDevice)) {
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

size_t checkDeviceExtensionSupport(VkPhysicalDevice device, ExtensionNames requiredExtensions) {
    Extensions deviceExtensions = {};
    vkEnumerateDeviceExtensionProperties(device, nullptr, &deviceExtensions.count, nullptr);
    deviceExtensions.items = (VkExtensionProperties*)malloc(deviceExtensions.count * sizeof(VkExtensionProperties));
    vkEnumerateDeviceExtensionProperties(device, nullptr, &deviceExtensions.count, deviceExtensions.items);

    // this will work if theres only one required extension.
    // if we need to add more, we probably need to better abstract this
    for (size_t i = 0; i < requiredExtensions.count; ++i){
        for (size_t j = 0; j < deviceExtensions.count; ++j) {
            const char* item_1 = requiredExtensions.items[i];
            const char* item_2 = deviceExtensions.items[j].extensionName;
            if (strcmp(item_1, item_2) == 0 ){
                free (deviceExtensions.items);
                return 1;
            }
        }

    }
    free (deviceExtensions.items);
    return 0;
}

size_t isDeviceSuitable(AppContext* appContext, VkPhysicalDevice device) {
    QueueFamilyIndices indices = findQueueFamilies(appContext, device);

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    // once again just hardcoding this for the sake of completing the tutorial
    ExtensionNames extensions = {.count = 1, .capacity = 5};
    extensions.items = (const char**)malloc(extensions.count * sizeof(const char**));
    extensions.items[0] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;



    if (indices.graphicsFamily.isPresent && deviceFeatures.geometryShader && checkDeviceExtensionSupport(device, extensions)) {
        free(extensions.items);
        return 1;
    }

    free(extensions.items);
    return 0;
}

QueueFamilyIndices findQueueFamilies(AppContext* appContext, VkPhysicalDevice device) {
    QueueFamilies queuefamilies = {};
    QueueFamilyIndices indices = {.graphicsFamily = {.isPresent = 0, .value = 0}, .presentFamily = {.isPresent = 0, .value = 0}};
    VkBool32 presentSupport = false;

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queuefamilies.count, nullptr);
    queuefamilies.capacity = queuefamilies.count * 2;
    queuefamilies.items = (VkQueueFamilyProperties*)malloc(queuefamilies.count * sizeof(VkQueueFamilyProperties));

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queuefamilies.count, queuefamilies.items);
    for (size_t i = 0; i < queuefamilies.count; ++i) {
        VkQueueFamilyProperties targetFamily = queuefamilies.items[i];
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, appContext->surface, &presentSupport);
        if (targetFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            opt_uint32_t some = {.isPresent = 1, .value = (uint32_t)i};
            indices.graphicsFamily = some;
        }

        if (presentSupport) {
            opt_uint32_t some = {.isPresent = 1, .value = (uint32_t)i};
            indices.presentFamily = some;
        }
    }
    free(queuefamilies.items);
    return indices;
}

#endif /* ifndef ILY_PHYSICAL_DEVICE_CONTEXT */

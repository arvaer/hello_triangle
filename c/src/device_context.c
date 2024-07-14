#ifndef ILY_DEVICE_CONTEXT
#define ILY_DEVICE_CONTEXT
#include "context.h"
#include "ily_errors.h"
#include <cstdio>
#include <cstdlib>
#include <vulkan/vulkan_core.h>

typedef struct {
    VkPhysicalDevice* items;
    uint32_t count;
    uint32_t capacity;
} PhysicalDevices;

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

    if (vkEnumeratePhysicalDevices(appContext->instance, &physicalDevices.count,
                                   physicalDevices.items) != VK_SUCCESS) {
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
    vkGetPhysicalDeviceProperties(appContext->physicalDevice,
                                  &deviceProperties);
    printf("Device Name: %s\n", deviceProperties.deviceName);
}

int isDeviceSuitable(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

     if (deviceFeatures.geometryShader){
         return 1;
     }
     return 0;
}

#endif /* ifndef ILY_DEVICE_CONTEXT */

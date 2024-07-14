#include <stdlib.h>
#ifndef ILY_CONTEXT
#define ILY_CONTEXT
#include "context.h"
#include <GLFW/glfw3.h>
#include <limits.h>
#include <string.h>
#include <vulkan/vulkan_core.h>

// __ Validation Layers __
typedef struct {
    VkLayerProperties* layerNames;
    uint32_t count;
    size_t capacity;
} AvailableLayers;


int checkValidationLayerSupport(RequiredLayers* requiredLayers) {
    // build available layers struct
    AvailableLayers availableLayers = {0};
    availableLayers.capacity = 256;
    vkEnumerateInstanceLayerProperties(&availableLayers.count, NULL);

    availableLayers.layerNames = (VkLayerProperties*)malloc(
      sizeof(VkLayerProperties) * availableLayers.count);
    if (!availableLayers.layerNames) return 0;


    vkEnumerateInstanceLayerProperties(&availableLayers.count,
                                       availableLayers.layerNames);

    // check that all the required layers are in the available layers
    for (size_t i = 0; i < requiredLayers->count; ++i) {
        int layerFound = 0;
        for (size_t j = 0; j < availableLayers.count; ++j) {
            if (strcmp(requiredLayers->layerNames[i],
                       availableLayers.layerNames[j].layerName) == 0) {
                layerFound = 1;
                break;
            }
        }
        if (layerFound == 0) {
            free(availableLayers.layerNames);
            return 0;
        }
    }
    free(availableLayers.layerNames);
    return 1;
}


RequiredLayers buildRequiredLayers() {
    // Honestly just going to hard code the desired layers in here.
    // at first I was passing in the args but theres no point
    const char* validationLayers[] = {"VK_LAYER_KHRONOS_validation"};
    // build required layers struct
    RequiredLayers requiredLayers = {0};
    requiredLayers.count = 1;
    requiredLayers.capacity = UINT_MAX;
    requiredLayers.layerNames = validationLayers;
    return requiredLayers;
}

#endif /* ifndef ILY_CONTEXT */

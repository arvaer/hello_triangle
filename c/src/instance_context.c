#ifndef ILY_CONTEXT
#define ILY_CONTEXT
#include "context.h"
#include <GLFW/glfw3.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan_core.h>
// __ Validation Layers __
typedef struct {
    VkLayerProperties* items;
    uint32_t count;
    size_t capacity;
} AvailableLayers;

typedef struct {
    const char** items;
    size_t count;
    size_t capacity;
} RequiredLayers;

// forward declared stuff
void populateLayers(RequiredLayers* requiredLayers);
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* debugInfo);
int checkValidationLayerSupport(RequiredLayers* requiredLayers);

// main logic
void createInstance(AppContext* appContext) {
    RequiredLayers validationLayers = {};
    populateLayers(&validationLayers);
    if (enableValidationLayers &&
        !checkValidationLayerSupport(&validationLayers)) {
        appContext->fp_errBack(ILY_FAILED_TO_ENABLE_VALIDATION_LAYERS);
    }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    VkDebugUtilsMessengerCreateInfoEXT debugInfo = {};
    populateDebugMessengerCreateInfo(&debugInfo);
    createInfo.pNext = &debugInfo;

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = (uint32_t)validationLayers.count;
        createInfo.ppEnabledLayerNames = validationLayers.items;
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&createInfo, NULL, &(*appContext).instance) !=
        VK_SUCCESS) {
        appContext->fp_errBack(ILY_FAILED_TO_CREATE_INSTANCE);
        exit(1);
    };
}

int checkValidationLayerSupport(RequiredLayers* requiredLayers) {

    AvailableLayers availableLayers = {0};
    vkEnumerateInstanceLayerProperties(&availableLayers.count, NULL);

    availableLayers.items = (VkLayerProperties*)malloc( sizeof(VkLayerProperties) * availableLayers.count);
    if (!availableLayers.items) {
        printf("Failed to allocate memory for layer names.\n");
        return 0;
    }

    vkEnumerateInstanceLayerProperties(&availableLayers.count, availableLayers.items);

    // Check all the required layers are in the available layers
    for (size_t i = 0; i < requiredLayers->count; ++i) {
        int layerFound = 0;

        for (size_t j = 0; j < availableLayers.count; ++j) {
            if (strcmp(requiredLayers->items[i], availableLayers.items[j].layerName) == 0) {
                layerFound = 1;
                break;
            }
        }

        if (layerFound == 0) {
            free(availableLayers.items);
            return 0;
        }
    }

    free(availableLayers.items);
    return 1;
}

void populateLayers(RequiredLayers* requiredLayers) {
    // Honestly just going to hard code the desired layers in here.
    // at first I was passing in the args but theres no point
    static const char* validationLayers[] = {"VK_LAYER_KHRONOS_validation"};
    // build required layers struct
    requiredLayers->count = 1;
    requiredLayers->capacity = 1;
    requiredLayers->items = validationLayers;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT messageType,
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    fprintf(stderr, "Validation layer: %s\n", pCallbackData->pMessage);
    return VK_FALSE;
}
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* debugCreateInfo) {
    debugCreateInfo->sType =
      VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo->messageSeverity =
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugCreateInfo->messageType =
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugCreateInfo->pfnUserCallback = debugCallback;
}

#endif /* ifndef ILY_CONTEXT */

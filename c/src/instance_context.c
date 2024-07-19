#ifndef ILY_CONTEXT
#define ILY_CONTEXT
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "context.h"
#include "ily_types.h"

// __ Validation Layers __
static const char* validationLayers[] = {"VK_LAYER_KHRONOS_validation"};
const int preLayerCount = 1;

// forward declared stuff
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* debugInfo);
int checkValidationLayerSupport(vector* requiredLayers);

// main logic
void createInstance(AppContext* appContext) {
    vector requiredLayers = {};
    vector_init(&requiredLayers, sizeof(char));
    for (int i = 0; i < preLayerCount; ++i) {
        vector_append(&requiredLayers, validationLayers[i]);
    }

    if (enableValidationLayers &&
        !checkValidationLayerSupport(&requiredLayers)) {
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
        createInfo.enabledLayerCount = requiredLayers.count;
        createInfo.ppEnabledLayerNames = (const char* const*)requiredLayers.items;
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&createInfo, NULL, &(*appContext).instance) !=
        VK_SUCCESS) {
        appContext->fp_errBack(ILY_FAILED_TO_CREATE_INSTANCE);
        exit(1);
    };
}

int checkValidationLayerSupport(vector* requiredLayers) {
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    VkLayerProperties layers[layerCount];

    vkEnumerateInstanceLayerProperties(&layerCount, layers);
    // Check all the required layers are in the available layers
    for (size_t i = 0; i < requiredLayers->count; ++i) {
        int layerFound = 0;

        for (size_t j = 0; j < layerCount; ++j) {
            const char** pRequiredLayerNames = (const char**)(requiredLayers->items);
            const char* requiredLayerName = *pRequiredLayerNames;

            if (strcmp(requiredLayerName, layers[j].layerName) == 0) {
                layerFound = 1;
                break;
            }
        }

        if (layerFound == 0) {
            return 0;
        }
    }
    return 1;
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

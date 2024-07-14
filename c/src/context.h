#ifndef ILY_CONTEXT_H
#define ILY_CONTEXT_H

#include <stdio.h>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
#include "../lib/ily_errors.h"

#ifdef NDEBUG
const int enableValidationLayers = 0;
#else
const int enableValidationLayers = 1;
#endif

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT messageType,
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    fprintf(stderr, "Validation layer: %s\n", pCallbackData->pMessage);

    return VK_FALSE;
}

typedef struct AppContext {
    GLFWwindow* window;
    VkInstance instance;
    ErrorCallback fp_errBack;
} AppContext;

typedef struct {
    const char** layerNames;
    size_t count;
    size_t capacity;
} RequiredLayers;

int checkValidationLayerSupport(RequiredLayers* requiredLayers);
RequiredLayers buildRequiredLayers(void);
void createInstance(AppContext* appContext);

#endif // ILY_CONTEXT_H


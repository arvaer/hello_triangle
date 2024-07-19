#ifndef ILY_CONTEXT_H
#define ILY_CONTEXT_H
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include "ily_errors.h"
#include "ily_types.h"

#ifdef NDEBUG
const int enableValidationLayers = 0;
#else
const size_t enableValidationLayers = 1;
#endif


typedef struct AppContext {
    GLFWwindow* window;
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSurfaceKHR surface;
    VkDebugUtilsMessengerEXT debugMessenger;
    ErrorCallback fp_errBack;
} AppContext;

typedef struct {
    option graphicsFamily;
    option presentFamily;
} QueueFamilyIndices;

typedef struct {
    const char** items;
    uint32_t count;
    uint32_t capacity;
} ExtensionNames;

void createInstance(AppContext* appContext);
void setupDebugMessenger(AppContext* appContext);
void pickPhysicalDevice(AppContext* appContext);
void createLogicalDevice(AppContext* appContext);
void createSurface(AppContext* appContext);

QueueFamilyIndices findQueueFamilies(AppContext* appContext, VkPhysicalDevice device);

#endif // ILY_CONTEXT_H

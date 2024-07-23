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

typedef struct SwapchainContext {
    VkSwapchainKHR swapchain;
    vector swapchainImages;
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;
} SwapchainContext;

typedef struct AppContext {
    GLFWwindow* window;
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSurfaceKHR surface;
    SwapchainContext swapchainContext;
    VkDebugUtilsMessengerEXT debugMessenger;
    ErrorCallback fp_errBack;
} AppContext;

typedef struct {
    option graphicsFamily;
    option presentFamily;
} QueueFamilyIndices;

typedef struct {
    VkSurfaceCapabilitiesKHR capabilities;
    vector formats;
    vector presentModes;
} SwapChainSupportDetails;

void createInstance(AppContext* appContext);
void setupDebugMessenger(AppContext* appContext);
void pickPhysicalDevice(AppContext* appContext);
void createLogicalDevice(AppContext* appContext);
void createSurface(AppContext* appContext);
void createSwapchain(AppContext* appContext);

QueueFamilyIndices findQueueFamilies(AppContext* appContext, VkPhysicalDevice device);

#endif // ILY_CONTEXT_H

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

#define da_append(xs, x)                                                       \
    do {                                                                       \
        if (xs.count >= xs.capacity) {                                         \
            if (xs.capacity == 0) {                                            \
                xs.capacity = 5;                                               \
            } else {                                                           \
                xs.capacity *= 2;                                              \
                xs.items = realloc(xs.items, xs.capacity * sizeof(*xs.items)); \
            }                                                                  \
            xs.items[xs.count++] = x;                                          \
        }                                                                      \
    } while (0)

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
    opt_uint32_t graphicsFamily;
    opt_uint32_t presentFamily;
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

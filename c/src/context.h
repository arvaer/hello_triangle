#ifndef ILY_CONTEXT_H
#define ILY_CONTEXT_H
#include "ily_errors.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#ifdef NDEBUG
const int enableValidationLayers = 0;
#else
const int enableValidationLayers = 1;
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
            xs.items[xs.count++] = x                                           \
        }                                                                      \
    } while (0)

typedef struct AppContext {
    GLFWwindow* window;
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    ErrorCallback fp_errBack;
} AppContext;

void createInstance(AppContext* appContext);
void pickPhysicalDevice(AppContext* appContext);

#endif // ILY_CONTEXT_H

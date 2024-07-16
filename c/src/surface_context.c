#ifndef ILY_SURFACE_CONTEXT
#define ILY_SURFACE_CONTEXT
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "context.h"
#include "ily_errors.h"

void createSurface(AppContext* appContext) {
    if (glfwCreateWindowSurface(appContext->instance, appContext->window, nullptr, &appContext->surface)!= VK_SUCCESS) {
        appContext->fp_errBack(ILY_FAILED_TO_CREATE_WINDOW_SURFACE);
    }
}

#endif /* ifndef ILY_SURFACE_CONTEXT */

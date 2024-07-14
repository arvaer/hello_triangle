#include "../lib/ily_errors.h"
#include "context.h"
#include <assert.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

// __ VULKAN __
void initVulkan(AppContext* appContext);
void mainLoop(AppContext* appContext);
void cleanup(AppContext* appContext);

// ___ GLFW ___
void initWindow(AppContext* appContext);

void run(AppContext* appContext) {
    createInstance(appContext);
    initWindow(appContext);
    initVulkan(appContext);
    pickPhysicalDevice(appContext);
    mainLoop(appContext);
    cleanup(appContext);
}

void initVulkan(AppContext* appContext) {}

void mainLoop(AppContext* appContext) {
    while (!glfwWindowShouldClose((*appContext).window)) {
        glfwPollEvents();
    }
}

void cleanup(AppContext* appContext) {
    glfwDestroyWindow((*appContext).window);
    glfwTerminate();
}

void initWindow(AppContext* appContext) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    (*appContext).window =
      glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", NULL, NULL);
    assert((*appContext).window != NULL);
}


int main() {
    AppContext* appContext = (AppContext*)malloc(sizeof(AppContext));

    appContext->fp_errBack = &printError;
    appContext->window = NULL;
    appContext->instance = NULL;
    appContext->physicalDevice = VK_NULL_HANDLE;

    run(appContext);
    free(appContext);
    return EXIT_SUCCESS;
}

#include "../lib/ily_errors.h"
#include "context.h"
#include "graphics_pipeline.h"
#include <assert.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

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
    initWindow(appContext);
    initVulkan(appContext);
    createSurface(appContext);
    mainLoop(appContext);
    cleanup(appContext);
}

void initVulkan(AppContext* appContext) {
    createInstance(appContext);
    createSurface(appContext);
    pickPhysicalDevice(appContext);
    createLogicalDevice(appContext);
    createSwapchain(appContext);
    createImageViews(appContext);
    createGraphicsPipeline(appContext);
}

void mainLoop(AppContext* appContext) {
    while (!glfwWindowShouldClose((*appContext).window)) {
        glfwPollEvents();
    }
}

void cleanup(AppContext* appContext) {
    for (size_t i = 0; i < appContext->swapchainContext.swapchainImageViews.count; ++i){
       VkImageView imageView = *((VkImageView*)vector_get(&appContext->swapchainContext.swapchainImageViews, i));
       vkDestroyImageView(appContext->logicalDevice, imageView, nullptr);
    }

    vkDestroySwapchainKHR(appContext->logicalDevice, appContext->swapchainContext.swapchain, nullptr);
    vkDestroySurfaceKHR(appContext->instance, appContext->surface, nullptr);
    vkDestroyDevice(appContext->logicalDevice, nullptr);

    glfwDestroyWindow(appContext->window);
    glfwTerminate();
}

void initWindow(AppContext* appContext) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    (*appContext).window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", NULL, NULL);
    assert((*appContext).window != NULL);
}

int main() {
    AppContext* appContext = (AppContext*)malloc(sizeof(AppContext));

    appContext->window = NULL;
    appContext->instance = NULL;
    appContext->physicalDevice = VK_NULL_HANDLE;
    appContext->logicalDevice = VK_NULL_HANDLE;
    appContext->graphicsQueue = NULL;
    appContext->presentQueue = NULL;
    appContext->surface = NULL;
    appContext->swapchainContext = {};
    appContext->debugMessenger = NULL;
    appContext->fp_errBack = &printError;
    run(appContext);

    free(appContext);
    return EXIT_SUCCESS;
}

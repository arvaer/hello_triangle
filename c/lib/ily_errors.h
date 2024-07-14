#ifndef ILY_ERRORS_H
#define ILY_ERRORS_H

typedef enum {
    ILY_FAILED_TO_CREATE_INSTANCE,
    ILY_FAILED_TO_ENABLE_VALIDATION_LAYERS,
    ILY_FAILED_TO_FIND_GPU_WITH_VULKAN_SUPPORT
} ERROR;

typedef void (*ErrorCallback)(ERROR error);

void printError(ERROR error);
                                                                            \

#endif // ILY_ERRORS_H


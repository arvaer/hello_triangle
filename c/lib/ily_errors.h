#ifndef ILY_ERRORS_H
#define ILY_ERRORS_H
// Idk if this is a good way to handle errors in C. Basically this is an attempt at creating a system similar to
// the std::runtime_exceptions in the main tutorial. Essentionally, we pass in a functionpointer to printError for out app context.
// then we patternmatch on the actual error and use that to get some information in stderr
typedef enum {
    ILY_FAILED_TO_CREATE_INSTANCE,
    ILY_FAILED_TO_ENABLE_VALIDATION_LAYERS,
    ILY_FAILED_TO_FIND_GPU_WITH_VULKAN_SUPPORT
} ERROR;

typedef void (*ErrorCallback)(ERROR error);

void printError(ERROR error);

#endif // ILY_ERRORS_H

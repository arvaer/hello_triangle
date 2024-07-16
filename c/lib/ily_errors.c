#include "ily_errors.h"
#include <stdio.h>
#include <stdlib.h>

void printError(ERROR error) {
    switch (error) {
    case ILY_FAILED_TO_CREATE_INSTANCE:
        fprintf(stderr, "FAILED TO CREATE ILY INSTANCE\n");
        break;
    case ILY_FAILED_TO_ENABLE_VALIDATION_LAYERS:
        fprintf(stderr, "VALIDATION LAYERS REQUESTED BUT NOT AVAILABLE\n");
        break;
    case ILY_FAILED_TO_FIND_GPU_WITH_VULKAN_SUPPORT:
        fprintf(stderr, "FAILED TO FIND A GPU(S) WITH VULKAN SUPPORT\n");
        break;
    case ILY_GRAPHICS_FAMILY_NOT_PRESENT:
        fprintf(stderr, "GRAPHICS FAMILY UNAVAILABLE WHILE UNWRAPPING INDICES\n");
        break;
    case ILY_LOGICAL_DEVICE_CREATION_FAILED:
        fprintf(stderr, "LOGICAL DEVICE CREATION FAILED\n");
        break;
    case ILY_FAILED_TO_CREATE_DEBUG_MESSENGER:
        fprintf(stderr, "FAILED TO CREATE DEBUG MESSENGER\n");
        break;
    default:
        fprintf(stderr, "UNKNOWN ERROR\n");
        break;
    }

    fprintf(stderr, "PROGRAM ERRORED OUT. NOW EXITING \r\n");
    exit(1);
}

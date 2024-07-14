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
    default:
        fprintf(stderr, "UNKNOWN ERROR\n");
        break;
    }

    fprintf(stderr, "PROGRAM ERRORED OUT. NOW EXITING \r\n");
    exit(1);
}

#include "ily_errors.h"
#include <stdio.h>

void printError(ERROR error) {
    switch (error) {
    case ILY_FAILED_TO_CREATE_INSTANCE:
        fprintf(stderr, "FAILED TO CREATE ILY INSTANCE\n");
        break;
    case ILY_FAILED_TO_ENABLE_VALIDATION_LAYERS:
        fprintf(stderr, "VALIDATION LAYERS REQUESTED BUT NOT AVAILABLE");
        break;
    default:
        fprintf(stderr, "UNKNOWN ERROR\n");
        break;
    }
}

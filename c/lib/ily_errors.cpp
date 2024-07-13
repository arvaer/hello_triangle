#include "ily_errors.h"
#include <stdio.h>

void printError(ERROR error) {
    switch (error) {
        case ILY_FAILED_TO_CREATE_INSTANCE:
            printf("FAILED TO CREATE ILY INSTANCE\n");
            break;
        default:
            printf("UNKNOWN ERROR\n");
            break;
    }
}


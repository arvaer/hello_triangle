#ifndef ILY_ERRORS_H
#define ILY_ERRORS_H

typedef enum {
    ILY_FAILED_TO_CREATE_INSTANCE
} ERROR;

typedef void (*ErrorCallback)(ERROR error);

void printError(ERROR error);

#endif // ILY_ERRORS_H


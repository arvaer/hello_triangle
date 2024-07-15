#ifndef ILY_TYPES_H
#define ILY_TYPES_H
#include <stdint.h>
// There are sections of the vulkan tutorial that call out to the std::optional from cpp
// After some research online, I think it would be straight forward to implement this using some generics
// from c11 (atleast thats what the reddit post said.)
// It's also possible to define a macro that will generate option structs around a given type you pass it.
// anyways, if you come back to this, the reason for just coding out all of the optional types we encounter
// is because we can. maybe in the future we can try making our own optional monad but theres enough
// complexity here that we dont need to add stff. certainly not in C where there are not generics out of the box
// it's also then easy to just implement unwrap we can also implement unwrap from here without polluting the src code

typedef struct {
    int isPresent;
    uint32_t value;
} opt_uint32_t;
#endif // !ILY_TYPES_H

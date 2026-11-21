#include <nitro/types.h>

extern u8 gCamera;

void *Camera_GetPrimary(void) {
    return &gCamera;
}

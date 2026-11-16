#include <nitro/types.h>

typedef struct UnkStruct_0233b610 {
    u8 pad[0xc8];
    u32 unkC8;
} UnkStruct_0233b610;

extern UnkStruct_0233b610 gCamera;
extern u8 data_ov000_0236ab3c;

void *Camera_GetOtherBySwapState(void) {
    if ((gCamera.unkC8 & 2) == 0)
        return &data_ov000_0236ab3c;
    return &gCamera;
}

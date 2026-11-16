#include <nitro/types.h>

typedef struct UnkStruct_0233b62c {
    u8 pad[0xc8];
    u32 unkC8;
} UnkStruct_0233b62c;

extern UnkStruct_0233b62c gCamera;
extern u8 data_ov000_0236ab3c;

void *func_ov000_0233b62c(void) {
    if ((gCamera.unkC8 & 2) != 0)
        return &data_ov000_0236ab3c;
    return &gCamera;
}

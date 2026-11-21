#include <nitro/types.h>

typedef struct UnkStruct_0233b1b8 {
    u32 unk0;
    u8 pad[0x60];
} UnkStruct_0233b1b8;

extern UnkStruct_0233b1b8 gCamera[];

void Camera_LockX(s32 i) {
    gCamera[i].unk0 |= 0x40;
}

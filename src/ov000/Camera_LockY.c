#include <nitro/types.h>

typedef struct UnkStruct_0233b198 {
    u32 unk0;
    u8 pad[0x60];
} UnkStruct_0233b198;

extern UnkStruct_0233b198 gCamera[];

void Camera_LockY(s32 i) {
    gCamera[i].unk0 |= 0x80;
}

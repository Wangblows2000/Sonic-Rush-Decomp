#include <nitro/types.h>

typedef struct UnkStruct_0233b178 {
    u32 unk0;
    u8 pad[0x60];
} UnkStruct_0233b178;

extern UnkStruct_0233b178 gCamera[];

void Camera_UnlockX(s32 i) {
    gCamera[i].unk0 &= ~0x40;
}

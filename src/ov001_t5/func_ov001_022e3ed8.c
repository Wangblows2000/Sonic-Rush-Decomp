#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
} UnkPair_022e3ed8;

typedef struct {
    u8 pad0[0x110];
    s32 count;
    s32 state[8];
    UnkPair_022e3ed8 pos[8];
} UnkGroup_022e3ed8;

#pragma thumb on
void func_ov001_022e3ed8(u8 *p, s32 idx) {
    s32 i;

    for (i = 0; i < 8; i++) {
        ((UnkGroup_022e3ed8 *)(p + idx * 0x64))->pos[i].x = 0;
        ((UnkGroup_022e3ed8 *)(p + idx * 0x64))->pos[i].y = 0;
        ((UnkGroup_022e3ed8 *)(p + idx * 0x64))->state[i] = 2;
        ((UnkGroup_022e3ed8 *)(p + idx * 0x64))->count = 0;
    }
}
#pragma thumb reset

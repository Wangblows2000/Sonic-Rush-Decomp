#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
} UnkOfs_022e9510;

typedef struct {
    UnkOfs_022e9510 ofs[4];
} UnkBg_022e9510;

extern UnkBg_022e9510 gDisplayMain;

void GX_SetGraphicsMode(s32 a, s32 b, s32 c);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

#pragma thumb on
void func_ov001_022e9510(void) {
    vu32 z;

    GX_SetGraphicsMode(1, 3, 1);
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & 0x43) | 0x8484;
    z = 0;
    MIi_CpuClear32(z, (void *)0x6002000, 0x1000);
    gDisplayMain.ofs[2].x = 0;
    gDisplayMain.ofs[2].y = 0;
}
#pragma thumb reset

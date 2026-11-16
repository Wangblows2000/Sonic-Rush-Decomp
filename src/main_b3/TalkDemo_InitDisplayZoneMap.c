#include <nitro/types.h>

typedef struct { u16 v[8]; } UnkMosaic_02060910;
extern UnkMosaic_02060910 gDisplayMain;

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void GX_SetGraphicsMode(s32 a, s32 b, s32 c);

#pragma thumb on
void TalkDemo_InitDisplayZoneMap(void) {
    vu32 zero = 0;
    MIi_CpuClear32(zero, (void *)0x06002000, 0xa000);
    GX_SetGraphicsMode(1, 5, 1);
    *(vu32 *)0x04000000 &= ~0x38000000;
    *(vu32 *)0x04000000 &= ~0x07000000;
    *(vu16 *)0x0400000a = (*(vu16 *)0x0400000a & 0x43) | 0x4604;
    *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & 0x43) | 0x4408;
    *(vu16 *)0x0400000a = (*(vu16 *)0x0400000a & ~3) | 0;
    *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & ~3) | 1;
    *(vu16 *)0x04000008 = (*(vu16 *)0x04000008 & ~3) | 2;
    *(vu16 *)0x0400000e = (*(vu16 *)0x0400000e & ~3) | 3;
    *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0x1f00) | 0x1f00;
    gDisplayMain.v[2] = gDisplayMain.v[3] = gDisplayMain.v[4] = gDisplayMain.v[5] = 0;
}
#pragma thumb reset

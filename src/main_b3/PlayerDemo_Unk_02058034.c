#include <nitro/types.h>

typedef struct { u16 v[8]; } UnkMosaic_02058034;
extern UnkMosaic_02058034 gDisplayMain;
extern UnkMosaic_02058034 gDisplaySub;

void Stage_InitDisplayBoss(s32 a);
void GX_SetGraphicsMode(s32 a, s32 b, s32 c);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

#pragma thumb on
static inline void Clear32_02058034(void *dst, u32 size) {
    vu32 z = 0;
    MIi_CpuClear32(z, dst, size);
}

static inline u32 GetPlane_02058034(void) {
    return (*(vu32 *)0x04000000 & 0x1f00) >> 8;
}

static inline void SetPlane_02058034(u32 v) {
    *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0x1f00) | (v << 8);
}

void PlayerDemo_Unk_02058034(void) {
    Stage_InitDisplayBoss(0);
    GX_SetGraphicsMode(1, 5, 1);
    *(vu32 *)0x04000000 &= ~0x38000000;
    *(vu32 *)0x04000000 &= ~0x07000000;
    Clear32_02058034((void *)0x0600c000, 0x4000);
    Clear32_02058034((void *)0x06000000, 0x1000);
    Clear32_02058034((void *)0x06200000, 0x3000);
    *(vu16 *)0x0400000e = (*(vu16 *)0x0400000e & 0x43) | 0x400c;
    *(vu16 *)0x0400000a = (*(vu16 *)0x0400000a & ~3) | 0;
    *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & ~3) | 1;
    *(vu16 *)0x0400000e = (*(vu16 *)0x0400000e & ~3) | 2;
    *(vu16 *)0x04000008 = (*(vu16 *)0x04000008 & ~3) | 3;
    SetPlane_02058034(GetPlane_02058034() | 8);
    gDisplayMain.v[6] = gDisplayMain.v[7] = 0;
    *(vu16 *)0x04001008 = (*(vu16 *)0x04001008 & 0x43) | 0x2400;
    *(vu16 *)0x04001008 = (*(vu16 *)0x04001008 & ~3) | 0;
    *(vu32 *)0x04001000 = (*(vu32 *)0x04001000 & ~0x1f00) | 0x1100;
    gDisplaySub.v[0] = gDisplaySub.v[1] = 0;
}
#pragma thumb reset

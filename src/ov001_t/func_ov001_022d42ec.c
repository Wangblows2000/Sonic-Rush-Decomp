#include <nitro/types.h>

extern u16 gDisplayMain[];
extern u16 gDisplaySub[];

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void Vram_SetBankConfig(s32 a);
void Vram_ResetTexAndPltt(void);
void VramPltt_Reset(void);
void func_02002ab0(s32 a, s32 b, s32 c);
void GXS_SetGraphicsMode(u32 mode);

#pragma thumb on
void func_ov001_022d42ec(void) {
    vu32 z0;
    vu32 z1;
    vu32 z2;
    vu32 z3;
    vu32 z4;
    vu32 z5;

    Vram_SetBankConfig(0);
    Vram_ResetTexAndPltt();
    VramPltt_Reset();
    *(vu16 *)0x4000304 = (*(vu16 *)0x4000304 & ~0x20e) | 0x20e;
    *(vu16 *)0x4000304 |= 0x8000;
    func_02002ab0(1, 0, 0);
    *(vu32 *)0x4000000 &= ~0x38000000;
    *(vu32 *)0x4000000 &= ~0x07000000;
    *(vu16 *)0x400000a = (*(vu16 *)0x400000a & 0x43) | 0x210;
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & 0x43) | 0x188;
    *(vu16 *)0x400000e = (*(vu16 *)0x400000e & 0x43) | 0x84;
    GXS_SetGraphicsMode(0);
    *(vu16 *)0x400100e = (*(vu16 *)0x400100e & 0x43) | 0x84;
    *(vu16 *)0x4000008 = (*(vu16 *)0x4000008 & ~3) | 0;
    *(vu16 *)0x400000a = (*(vu16 *)0x400000a & ~3) | 1;
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & ~3) | 2;
    *(vu16 *)0x400000e = (*(vu16 *)0x400000e & ~3) | 3;
    *(vu16 *)0x4001008 = (*(vu16 *)0x4001008 & ~3) | 0;
    *(vu16 *)0x400100a = (*(vu16 *)0x400100a & ~3) | 1;
    *(vu16 *)0x400100c = (*(vu16 *)0x400100c & ~3) | 2;
    *(vu16 *)0x400100e = (*(vu16 *)0x400100e & ~3) | 3;
    gDisplayMain[0] = 0;
    gDisplayMain[1] = 0;
    gDisplayMain[2] = 0;
    gDisplayMain[3] = 0;
    gDisplayMain[4] = 0;
    gDisplayMain[5] = 0;
    gDisplayMain[6] = 0;
    gDisplayMain[7] = 0;
    gDisplaySub[0] = 0;
    gDisplaySub[1] = 0;
    gDisplaySub[2] = 0;
    gDisplaySub[3] = 0;
    gDisplaySub[4] = 0;
    gDisplaySub[5] = 0;
    gDisplaySub[6] = 0;
    gDisplaySub[7] = 0;
    z0 = 0;
    MIi_CpuClear32(z0, (void *)0x6000000, 0x80000);
    z1 = 0;
    MIi_CpuClear32(z1, (void *)0x6200000, 0x20000);
    z2 = 0;
    MIi_CpuClear32(z2, (void *)0x5000000, 0x200);
    z3 = 0;
    MIi_CpuClear32(z3, (void *)0x5000400, 0x200);
    z4 = 0;
    MIi_CpuClear32(z4, (void *)0x5000200, 0x200);
    z5 = 0;
    MIi_CpuClear32(z5, (void *)0x5000600, 0x200);
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0xc00;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1800;
}
#pragma thumb reset

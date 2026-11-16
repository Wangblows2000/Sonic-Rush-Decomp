#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s16 bright;
} UnkBright_022cf724;

typedef struct {
    u16 v[8];
} UnkScroll_022cf724;

extern UnkBright_022cf724 data_0208926c;
extern UnkBright_022cf724 data_02087d44;
extern UnkScroll_022cf724 data_0208922c;
extern UnkScroll_022cf724 data_02087d04;

void GXx_SetMasterBrightness_(vu16 *reg, s32 v);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

#pragma thumb on
void func_ov001_022cf724(void) {
    vu32 z0;
    vu32 z1;
    vu32 z2;
    vu32 z3;
    vu32 z4;
    vu32 z5;

    data_0208926c.bright = 0x10;
    data_02087d44.bright = 0x10;
    GXx_SetMasterBrightness_((vu16 *)0x400006c, data_0208926c.bright);
    GXx_SetMasterBrightness_((vu16 *)0x400106c, data_02087d44.bright);
    data_0208922c.v[0] = 0;
    data_0208922c.v[1] = 0;
    data_0208922c.v[2] = 0;
    data_0208922c.v[3] = 0;
    data_0208922c.v[4] = 0;
    data_0208922c.v[5] = 0;
    data_0208922c.v[6] = 0;
    data_0208922c.v[7] = 0;
    data_02087d04.v[0] = 0;
    data_02087d04.v[1] = 0;
    data_02087d04.v[2] = 0;
    data_02087d04.v[3] = 0;
    data_02087d04.v[4] = 0;
    data_02087d04.v[5] = 0;
    data_02087d04.v[6] = 0;
    data_02087d04.v[7] = 0;
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
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0;
}
#pragma thumb reset

#include <nitro/types.h>

typedef struct {
    s32 m[4];
} UnkMtx22_022d4ff0;

typedef struct {
    u16 v[8];
} UnkScroll_022d4ff0;

extern UnkScroll_022d4ff0 data_0208922c;
extern UnkScroll_022d4ff0 data_02087d04;
extern u8 data_02089254[];
extern u8 data_02089264[];
extern u8 data_02087d2c[];
extern u8 data_02087d3c[];

void func_02000cb4(UnkMtx22_022d4ff0 *m);
void func_02003d94(void *dst, UnkMtx22_022d4ff0 *m, s32 a, s32 b, s32 c, s32 d);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

#pragma thumb on
void func_ov001_022d4ff0(void) {
    vu32 z0;
    vu32 z1;
    vu32 z2;
    vu32 z3;
    vu32 z4;
    vu32 z5;
    UnkMtx22_022d4ff0 m;

    func_02000cb4(&m);
    data_0208922c.v[0] = 0;
    data_0208922c.v[1] = 0;
    data_0208922c.v[2] = 0;
    data_0208922c.v[3] = 0;
    data_0208922c.v[4] = 0;
    data_0208922c.v[5] = 0;
    data_0208922c.v[6] = 0;
    data_0208922c.v[7] = 0;
    func_02003d94(data_02089254, &m, 0, 0, 0, 0);
    func_02003d94(data_02089264, &m, 0, 0, 0, 0);
    data_02087d04.v[0] = 0;
    data_02087d04.v[1] = 0;
    data_02087d04.v[2] = 0;
    data_02087d04.v[3] = 0;
    data_02087d04.v[4] = 0;
    data_02087d04.v[5] = 0;
    data_02087d04.v[6] = 0;
    data_02087d04.v[7] = 0;
    func_02003d94(data_02087d2c, &m, 0, 0, 0, 0);
    func_02003d94(data_02087d3c, &m, 0, 0, 0, 0);
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

#include <nitro/types.h>

typedef struct { u8 unk0[0x28]; u8 bg2[0x10]; u8 bg3[0x10]; } UnkDisp_0203039c;
typedef struct { u8 unk0[0x80]; u32 unk80; u32 unk84; } UnkStruct_0203039c;

extern u32 data_02087cb8;
extern u32 data_02087cc4;
extern u32 data_02087cc0;
extern u8 data_02087cdc[];
extern u32 data_02087ccc[];
extern UnkDisp_0203039c *data_02072c78[];
extern u8 data_0208a754[];
extern UnkStruct_0203039c data_0208f754;
extern u32 data_02087cc8;
extern s32 data_02087cb4;
extern s32 data_02087cbc;
extern void *data_02087cb0;

void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_02000cb4(void *m);
void func_02003d94(void *dst, const void *m, s32 a, s32 b, s32 c, s32 d);

void func_0203039c(void) {
    vu16 z1;
    vu16 z2;
    vu16 z3;
    vu16 z4;
    fx32 m[4];
    u32 i;
    data_02087cb8 = 0;
    data_02087cc4 = 0;
    data_02087cc0 = *(u32 *)0x027FFC20;
    z1 = 0;
    MIi_CpuClear16(z1, data_02087cdc, 0x28);
    z2 = 0;
    MIi_CpuClear16(z2, data_02087ccc, 0x10);
    data_02087ccc[1] = 0;
    data_02087ccc[0] = 0;
    func_02000cb4(m);
    for (i = 0; i < 2; i++) {
        UnkDisp_0203039c *d;
        z3 = 0;
        d = data_02072c78[i];
        MIi_CpuClear16(z3, d, 0x1528);
        func_02003d94(d->bg2, m, 0, 0, 0, 0);
        func_02003d94(d->bg3, m, 0, 0, 0, 0);
    }
    z4 = 0;
    MIi_CpuClear16(z4, data_0208a754, 0x5088);
    data_0208f754.unk80 = 0;
    data_0208f754.unk84 = 1;
    data_02087cc8 = 0;
    data_02087cb4 = -1;
    data_02087cbc = -1;
    data_02087cb0 = NULL;
}

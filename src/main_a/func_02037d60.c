#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u32 unk10; u8 unk14; } UnkStruct_02037d60;
extern UnkStruct_02037d60 data_022b65b4;
extern u8 data_022b658c[];
void func_02037cac(void);
void MIi_CpuClear16(u16 data, void *dest, u32 size);

void func_02037d60(void) {
    vu16 z0, z1;
    if (data_022b65b4.unk10 & 1) {
        if (!(data_022b65b4.unk10 & 2)) return;
        func_02037cac();
    }
    z0 = 0;
    MIi_CpuClear16(z0, &data_022b65b4, 0x28);
    z1 = 0;
    MIi_CpuClear16(z1, data_022b658c, 0x28);
    data_022b65b4.unk14 = 1;
    data_022b65b4.unk10 |= 1;
}

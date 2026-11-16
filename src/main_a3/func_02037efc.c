#include <nitro/types.h>

typedef struct {
    u8 data[0x20];
} UnkBlock_02037efc;

extern u16 data_022b65dc;
extern UnkBlock_02037efc data_022b69e0[];
extern UnkBlock_02037efc *data_022b65e0[];

void MIi_CpuClear32(u32 data, void *dst, u32 size);

void func_02037efc(void) {
    s32 i;
    {
        vu32 zero = 0;
        data_022b65dc = 0;
        MIi_CpuClear32(zero, data_022b69e0, sizeof(UnkBlock_02037efc) * 256);
    }
    for (i = 0; i < 256; i++) {
        data_022b65e0[i] = &data_022b69e0[i];
    }
}

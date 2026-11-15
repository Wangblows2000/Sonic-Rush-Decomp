/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022c4dac[];
s32 func_0206376c();
void func_020485f0();

void func_02048608(u8 *p0) {
    s32 t0 = *(u8 *)((u8 *)(*(u32 *)((u8 *)p0 + 0x14) + (*(s8 *)((u8 *)p0 + 0x18) << 2)) + 0x1);
    *(u16 *)data_022c4dac = t0;
    func_0206376c(data_022c4dac, t0);
    *(u32 *)p0 = (u32)func_020485f0;
}

#pragma thumb reset

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02060e70();
s32 func_0201b348();
s32 func_0201bf1c();
s32 func_02036894();

void func_02060824(u8 *p0) {
    s32 t0 = func_02060e70(0);
    if ((t0 & 0x10) == 0) {
        s32 t1 = (*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x28) + 0x4) & 1);
        if (t1 == 0) {
            func_0201b348(1, t1);
            func_0201bf1c(((u8 *)p0 + 0x2c));
            func_0201bf1c(((u8 *)p0 + 0x30));
            s32 t2 = (*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x28) + 0x4) & 2);
            if (t2 == 0) {
                func_02036894(2, t2);
            }
        }
    }
}

#pragma thumb reset

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_0201c070();
s32 func_0201b348();
s32 func_0201bf1c();
s32 Sound_ResetHeap();

void func_0205761c(u8 *p0) {
    if ((*(u32 *)((u8 *)p0 + 0x4) & 0x10000000) == 0) {
        if (*(u32 *)((u8 *)p0 + 0x34) != 0) {
            func_0201c070(((u8 *)p0 + 0x34), 0);
        }
        s32 t0 = (*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x2c) + 0x4) & 2);
        if (t0 == 0) {
            func_0201b348(2, t0);
            func_0201bf1c(((u8 *)p0 + 0x30));
            func_0201bf1c(((u8 *)p0 + 0x34));
            Sound_ResetHeap();
        }
    }
}

#pragma thumb reset

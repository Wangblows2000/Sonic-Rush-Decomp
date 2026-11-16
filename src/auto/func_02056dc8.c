/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_02058350();
s32 func_020371c8();

void func_02056dc8(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t1 = func_02058350(*(u32 *)data_022b4574);
    *(u32 *)((u8 *)t0 + 0x4) = t1;
    if (*(u32 *)((u8 *)t0 + 0x4) == 0) {
        func_020371c8(*(u32 *)data_022b4574);
        return;
    }
    s32 t2 = *(u32 *)t0;
    if (t2 != 0) {
        ((s32 (*)())t2)(t0, t2);
    }
}

#pragma thumb reset

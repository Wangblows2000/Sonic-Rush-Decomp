/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_02057350();
s32 func_020371c8();
s32 func_0201c070();
s32 func_0201bf1c();
s32 func_02036754();

void func_0205755c(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t1 = func_02057350(*(u32 *)data_022b4574);
    if (t1 == 0) {
        func_020371c8(*(u32 *)data_022b4574);
        return;
    }
    *(u8 *)((u8 *)t0 + 0x9) = (*(s8 *)((u8 *)t0 + 9) - 0x1);
    if (*(s8 *)((u8 *)t0 + 9) <= 0) {
        *(u8 *)((u8 *)t0 + 0x9) = *(u8 *)((u8 *)t0 + 0x8);
        s32 t2 = *(u32 *)((u8 *)t0 + 0x4);
        if (*(u32 *)*(u32 *)((u8 *)t0 + 0x4) != 0) {
            func_0201c070(t2, 0);
            func_0201bf1c(*(u32 *)((u8 *)t0 + 0x4));
        }
        func_02036754(*(u32 *)((u8 *)t0 + 0x4), *(u8 *)((u8 *)t0 + 0xa), *(u8 *)((u8 *)t0 + 0xb));
    }
}

#pragma thumb reset

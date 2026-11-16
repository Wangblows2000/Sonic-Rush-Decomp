/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_0204f038();
s32 func_020371c8();

void func_0204f9f8(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    func_0204f038(*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10), (7 - *(u32 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10)), 8, 0x1000);
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    if ((u32)*(u32 *)t0 >= (u32)8) {
        func_020371c8(*(u32 *)data_022b4574);
    }
}

#pragma thumb reset

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_020371c8();

void func_02061780(void) {
    s32 t0 = *(u32 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    if (t0 != 0) {
        ((s32 (*)())t0)(t0);
    }
    func_020371c8(*(u32 *)data_022b4574);
}

#pragma thumb reset

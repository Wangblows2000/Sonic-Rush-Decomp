/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void func_02061780();
s32 func_020372d0();

void func_020617a4(s32 p0) {
    s32 t0 = func_020372d0((u32)func_02061780, 0, 7, 0xf000, 3, 4);
    s32 t1 = *(u32 *)((u8 *)t0 + 0x10);
    *(u32 *)t1 = p0;
}

#pragma thumb reset

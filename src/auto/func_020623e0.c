/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void func_02062340();
s32 func_020372d0();

void func_020623e0(void) {
    s32 t0 = func_020372d0((u32)func_02062340, 0, 7, 0xf00a, 3, 0);
}

#pragma thumb reset

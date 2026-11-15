/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02051628();
void func_02050408();
s32 func_020372d0();

void func_02051684(s32 p0) {
    func_02051628(0);
    s32 t0 = func_020372d0((u32)func_02050408, 0, 0, 0, 0, 4);
    s32 t1 = *(u32 *)((u8 *)t0 + 0x10);
    *(u32 *)t1 = p0;
}

#pragma thumb reset

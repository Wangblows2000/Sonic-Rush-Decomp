/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void func_ov000_0235f6e0();
s32 Task_Create();

s32 func_ov000_0235f740(s32 p0) {
    s32 t0 = Task_Create((u32)func_ov000_0235f6e0, 0, 0, 0x5000, 0x1000, 8);
    s32 t1 = *(u32 *)((u8 *)t0 + 0x10);
    *(u32 *)t1 = p0;
    *(u32 *)((u8 *)t1 + 0x4) = -1;
    return t0;
}

#pragma thumb reset

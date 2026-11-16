/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_ov001_022e6998();
void func_ov001_022e794c();

void func_ov001_022e5db0(s32 p0) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    *(u32 *)((u8 *)t0 + 0x74) = 0;
    *(u32 *)((u8 *)t0 + 0x18) = p0;
    s32 t1 = func_ov001_022e6998(*(u32 *)t0, *(u32 *)((u8 *)t0 + 0x14), *(u32 *)((u8 *)t0 + 0x18));
    *(u32 *)((u8 *)t0 + 0x1c) = t1;
    s32 t2 = *(u32 *)gTaskCurrent;
    *(u32 *)((u8 *)t2 + 0x8) = (u32)func_ov001_022e794c;
}

#pragma thumb reset

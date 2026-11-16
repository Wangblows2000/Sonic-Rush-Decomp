/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_ov001_022e2f00();
s32 func_ov001_022e2ee8();
s32 func_ov001_022e95b8();
void func_ov001_022e76ec();

void func_ov001_022e5e60(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    if (*(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) == 0) {
        func_ov001_022e2f00(*(u32 *)((u8 *)t0 + 0x4), t0);
    } else {
        func_ov001_022e2ee8(*(u32 *)((u8 *)t0 + 0x8));
    }
    func_ov001_022e95b8();
    *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_ov001_022e76ec;
}

#pragma thumb reset

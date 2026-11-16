/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022ebe50();
s32 func_ov001_022ebe4c();
s32 func_ov001_022ebdd8();
s32 func_ov001_022e9804();
s32 Touch_Disable();
extern u8 gPad[];
s32 Pad_Reset();

void func_ov001_022ebe98(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    func_ov001_022ebe50(*(u32 *)((u8 *)p0 + 0x10));
    func_ov001_022ebe4c(t0);
    func_ov001_022ebdd8(t0);
    func_ov001_022e9804();
    Touch_Disable();
    Pad_Reset(gPad, 0, 0);
}

#pragma thumb reset

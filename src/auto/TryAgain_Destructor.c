/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 Touch_Disable();
extern u8 gSystemFlags[];
s32 func_ov001_022d3e10();
s32 func_ov001_022d3dd8();
s32 func_ov001_022d3d64();
s32 func_ov001_022d3044();

void TryAgain_Destructor(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    Touch_Disable();
    s32 t1 = (*(u32 *)gSystemFlags & -1025);
    *(u32 *)gSystemFlags = t1;
    func_ov001_022d3e10(t0, gSystemFlags, t1);
    func_ov001_022d3dd8(t0);
    func_ov001_022d3d64(t0);
    func_ov001_022d3044();
}

#pragma thumb reset

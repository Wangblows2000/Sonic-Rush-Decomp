/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022e3398();
s32 func_ov001_022e8528();
s32 func_ov001_022e8514();
s32 func_ov001_022e849c();
extern u8 gPad[];
s32 Pad_Reset();
s32 Touch_Disable();
s32 SysSound_Release();

void func_ov001_022e8378(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    func_ov001_022e3398(*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x10) + 0x1cc4));
    *(u32 *)((u8 *)t0 + 0x1cc4) = 0;
    func_ov001_022e8528(t0, 0);
    func_ov001_022e8514(t0);
    func_ov001_022e849c(t0);
    Pad_Reset(gPad, 0, 0);
    Touch_Disable();
    SysSound_Release();
}

#pragma thumb reset

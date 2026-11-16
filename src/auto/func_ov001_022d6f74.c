/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void func_ov001_022d82dc();
s32 func_020372d0();
s32 func_ov001_022d7340();
s32 func_ov001_022dddd0();

void func_ov001_022d6f74(s32 p0) {
    s32 t0 = func_020372d0((u32)func_ov001_022d82dc, 0, 0, 0x2000, 0x1000, 4);
    *(u32 *)*(u32 *)((u8 *)t0 + 0x10) = p0;
    func_ov001_022d7340(p0);
    func_ov001_022dddd0(*(u32 *)((u8 *)p0 + 0x808), *(u32 *)((u8 *)p0 + 0x80c));
}

#pragma thumb reset

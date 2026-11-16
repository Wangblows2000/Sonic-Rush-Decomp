/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02056104();
s32 func_020563e4();
s32 func_020561e0();
s32 func_020563a4();
s32 func_02056844();
void func_02056840();

s32 func_02056890(u8 *p0) {
    func_02056104(p0, ((u8 *)p0 + 0x1334));
    func_020563e4(p0);
    func_020561e0(p0);
    func_020563a4(p0);
    s32 t0 = *(u32 *)((u8 *)p0 + 0x4);
    s32 t1 = *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x4) + 0x4);
    s32 t2 = (*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x4) + 0x4) & 0x100);
    if (t2 != 0) {
        *(u32 *)((u8 *)t0 + 0x4) = (t1 & -257);
        s32 t3 = func_02056844(p0, t0, -257, t2);
        return t3;
    }
    s32 v4 = t1;
    if ((t1 & 0x200) != 0) {
        *(u32 *)((u8 *)t0 + 0x4) = (v4 & -513);
        func_02056844(p0, t0, -513, (t1 & 0x200));
        *(u32 *)p0 = (u32)func_02056840;
        v4 = (s32)(u32)func_02056840;
    }
    return v4;
}

#pragma thumb reset

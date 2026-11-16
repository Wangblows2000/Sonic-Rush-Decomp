/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_020221fc();
s32 func_02002360();
s32 func_02022284();
s32 func_02022270();
s32 func_0202224c();
s32 func_02025dac();
s32 func_02025e1c();

void func_ov000_023568ac(s32 p0, s32 p1, s32 p2) {
    func_020221fc(1, 0, p1, 0, 0x1f, 0);
    func_02002360(p2, p2);
    func_02022284(0, *(s16 *)((u8 *)p2 + 0), *(s16 *)((u8 *)p2 + 2), *(s16 *)((u8 *)p2 + 4));
    func_02022270(0, 0x7fff);
    func_0202224c(0x7fff, 0x6f7b, 1);
    func_02025dac(p0, 0, 0xf);
    func_02025e1c(p0, 0, 0x40);
    func_02025e1c(p0, 0, 0x80);
    func_02025dac(p0, 0, 0xc0);
}

#pragma thumb reset

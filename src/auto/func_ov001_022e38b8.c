/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_020221fc();
s32 func_02022284();
s32 func_02022270();
s32 func_0202224c();
s32 func_02025dac();
s32 func_02025e1c();

void func_ov001_022e38b8(s32 p0, s32 p1) {
    s32 v0 = (s32)p1;
    if ((s32)p1 < 0) {
        v0 = 0;
    }
    s32 v1 = v0;
    if (v0 >= 0x20) {
        v1 = 0x1f;
    }
    func_020221fc(1, 0, 3, 0, v1, 0);
    func_02022284(0, -4096, -4096, -4096);
    func_02022284(1, 0xfff, -4096, -4096);
    func_02022284(2, 0xfff, 0xfff, -4096);
    func_02022284(3, -4096, 0xfff, -4096);
    func_02022270(0, 0x7fff);
    func_02022270(1, 0x2108);
    func_02022270(2, 0x2108);
    func_02022270(3, 0);
    func_0202224c(0x7fff, 0x5ef7, 1);
    func_02025dac(p0, 0, 0xf);
    func_02025e1c(p0, 0, 0x40);
    func_02025e1c(p0, 0, 0x80);
    func_02025dac(p0, 0, 0x1f0000);
}

#pragma thumb reset

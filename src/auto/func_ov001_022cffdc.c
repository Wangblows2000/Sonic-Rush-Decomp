/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 _fmul();
s32 _fdiv();
s32 _fsub();

void func_ov001_022cffdc(u8 *p0, u8 *p1, s32 p2, s32 p3) {
    if ((s32)p0 != 0) {
        s32 t0 = _fmul(0x40000000, p3);
        s32 t1 = _fdiv(t0, p2);
        *(u32 *)p0 = t1;
    }
    if ((s32)p1 != 0) {
        s32 t2 = _fmul(0x40000000, p3);
        s32 t3 = _fsub(0, t2);
        s32 t4 = _fmul(p2, p2);
        s32 t5 = _fdiv(t3, t4);
        *(u32 *)p1 = t5;
    }
}

#pragma thumb reset

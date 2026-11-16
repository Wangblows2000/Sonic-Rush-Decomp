/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_022c6ac4();
s32 func_ov001_022c69ac();
void func_ov001_022c75d8();
s32 func_ov001_022c7a34();
void func_ov001_022c689c();
s32 Task_Create();
s32 _fflt();
s32 _fdiv();
s32 func_0202e5e4();
s32 _fsub();
s32 _fmul();
s32 _fadd();
s32 _ffix();
extern u8 gDisplayMain[];

void func_ov001_022c7678(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0xc);
    if (t0 >= 5) {
        func_ov001_022c6ac4(p0, (t0 - 0x5), 0x3c, 1, 0);
    }
    s32 t1 = *(u32 *)((u8 *)p0 + 0xc);
    if (t1 >= 0x299) {
        s32 t2 = func_ov001_022c69ac(p0, (t1 - 0x299), 0x1e, 1, 0);
        if (t2 != 0) {
            func_ov001_022c7a34(p0, (u32)func_ov001_022c75d8);
            return;
        }
    }
    if (*(u32 *)((u8 *)p0 + 0xc) == 0x1a4) {
        Task_Create((u32)func_ov001_022c689c, 0, 0, 0xf200, 0x1173, 0);
    }
    s32 t3 = *(u32 *)((u8 *)p0 + 0xc);
    if (t3 < 0x1e0) {
        return;
    }
    s32 t4 = _fflt((t3 - 0x1e0));
    s32 t5 = _fdiv(t4, 0x42700000);
    s32 t6 = func_0202e5e4(t5, 0x3f800000);
    s32 v7 = t5;
    if (t6 != 0) {
        v7 = 0x3f800000;
    }
    s32 t8 = _fsub(0x3f800000, v7);
    s32 t9 = _fmul(t8, t8);
    s32 t10 = _fmul(t8, t9);
    s32 t11 = _fsub(0x3f800000, t10);
    s32 t12 = _fmul(0x428c0000, t11);
    _fadd(0xc28c0000, t12);
    s32 t13 = _ffix();
    *(u16 *)((u8 *)gDisplayMain + 0x8) = t13;
    *(u16 *)((u8 *)gDisplayMain + 0xa) = 0;
}

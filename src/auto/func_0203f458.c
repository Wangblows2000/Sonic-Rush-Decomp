/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_020405bc();
s32 func_020405e0();
extern u8 data_022c1940[];
extern u8 data_022c1994[];
s32 func_0203f3e4();
s32 func_0203f2ac();
extern u8 data_022c197c[];
extern u8 data_02078914[];

void func_0203f458(u8 *p0) {
    s32 t0 = *(u16 *)((u8 *)p0 + 0x2);
    if (t0 != 0) {
        func_020405bc(t0);
        return;
    }
    func_020405e0(1);
    if (*(u16 *)data_022c1940 == 0) {
        return;
    }
    s32 t1 = *(u32 *)data_022c1994;
    if (t1 != 0) {
        s32 t2 = func_0203f3e4(t1);
        if (t2 != 0) {
            return;
        }
        func_020405e0(9);
        return;
    }
    s32 t3 = func_0203f2ac();
    if (t3 != 0) {
        return;
    }
    s32 t4 = *(u32 *)data_022c197c;
    if (t4 != 0) {
        ((s32 (*)())t4)(data_02078914, t4);
    }
    func_020405e0(9);
}

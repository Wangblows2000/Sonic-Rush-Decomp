/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_020405bc();
s32 func_020405e0();
s32 func_0203f2ac();
extern u8 data_022c197c[];
extern u8 data_020788e4[];

void func_0203f370(u8 *p0) {
    s32 t0 = *(u16 *)((u8 *)p0 + 0x2);
    if (t0 != 0) {
        func_020405bc(t0);
        func_020405e0(9);
        return;
    }
    s32 t1 = func_0203f2ac();
    if (t1 != 0) {
        return;
    }
    s32 t2 = *(u32 *)data_022c197c;
    if (t2 != 0) {
        ((s32 (*)())t2)(data_020788e4, t2);
    }
    func_020405e0(9);
}

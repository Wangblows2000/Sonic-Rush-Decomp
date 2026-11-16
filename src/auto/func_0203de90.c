/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c1970[];
void func_0203ea08();
s32 func_020114f0();
s32 func_020405e0();
extern u8 data_022c197c[];
extern u8 data_02078708[];

void func_0203de90(s32 p0) {
    *(u32 *)data_022c1970 = p0;
    s32 t0 = func_020114f0(0xe, (u32)func_0203ea08, 0);
    if (t0 == 0) {
        return;
    }
    func_020405e0(9);
    s32 t1 = *(u32 *)data_022c197c;
    if (t1 == 0) {
        return;
    }
    ((s32 (*)())t1)(data_02078708, t1);
}

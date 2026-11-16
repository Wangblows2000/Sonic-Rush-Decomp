/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c1964[];
extern u8 data_022c1da0[];
s32 DC_FlushRange();
void func_0203eaa0();
s32 func_02011c44();
extern u8 data_022c197c[];
s32 func_02040674();
extern u8 data_020787b0[];

s32 func_0203eb10(s32 p0, s32 p1, s32 p2) {
    DC_FlushRange(data_022c1da0, *(u32 *)data_022c1964, p2, data_022c1964);
    s32 t0 = func_02011c44((u32)func_0203eaa0, p2, p0, p1, 0xffff, 0xe, 2);
    if (t0 != 2) {
        s32 t1 = *(u32 *)data_022c197c;
        if (t1 != 0) {
            s32 t2 = func_02040674(t0, t1);
            ((s32 (*)())*(u32 *)data_022c197c)(data_020787b0, t2, *(u32 *)data_022c197c);
        }
        return 0;
    }
    return 1;
}

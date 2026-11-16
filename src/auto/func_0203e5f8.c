/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c1980[];
s32 OS_Terminate();
s32 func_020405e0();
extern u8 data_022c1954[];
s32 func_0203e510();
extern u8 data_022c197c[];
extern u8 data_022c1960[];
extern u8 data_02078780[];

s32 func_0203e5f8(void) {
    s32 t0 = *(u32 *)data_022c1980;
    if (t0 != 7) {
        OS_Terminate(t0);
    }
    func_020405e0(1);
    s32 t1 = func_0203e510(*(u16 *)data_022c1954);
    s32 t2 = *(u32 *)data_022c197c;
    *(u16 *)data_022c1960 = t1;
    if (t2 != 0) {
        ((s32 (*)())t2)(data_02078780, *(u16 *)data_022c1960, data_022c1960, t2);
    }
    return *(u16 *)data_022c1960;
}

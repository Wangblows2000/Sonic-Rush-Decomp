/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c2440[];
extern u8 data_022c1ba0[];
s32 func_020125d8();
extern u8 data_022c197c[];
extern u8 data_020786dc[];
s32 func_020405bc();

s32 func_0203ddd8(s32 p0) {
    s32 t0 = func_020125d8(data_022c2440, p0, data_022c1ba0);
    if (t0 == 7) {
        return 1;
    }
    if (t0 == 5) {
        s32 t1 = *(u32 *)data_022c197c;
        if (t1 != 0) {
            ((s32 (*)())t1)(data_020786dc, t1);
        }
        func_020405bc(t0);
        return 0;
    }
    if (t0 == 0) {
        return 1;
    }
    func_020405bc(t0);
    return 0;
}

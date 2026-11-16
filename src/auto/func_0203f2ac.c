/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c1980[];
extern u8 data_022c197c[];
extern u8 data_020788b8[];
s32 func_020405e0();
extern u8 data_022c1994[];
extern u8 data_022c1ae0[];
extern u8 data_022c19b4[];
void func_0203f108();
s32 func_020116a0();
s32 func_020405bc();

s32 func_0203f2ac(void) {
    if ((u32)(*(u32 *)data_022c1980 - 0x4) <= (u32)2) {
        s32 t0 = *(u32 *)data_022c197c;
        if (t0 != 0) {
            ((s32 (*)())t0)(data_020788b8, t0);
        }
        return 1;
    }
    func_020405e0(3);
    s32 t1 = func_020116a0((u32)func_0203f108, data_022c1ae0, data_022c19b4, 1, ((u32)((*(u32 *)data_022c1994 != 0) << 0x10) >> 0x10));
    if (t1 == 2) {
        return 1;
    }
    func_020405bc(t1);
    return 0;
}

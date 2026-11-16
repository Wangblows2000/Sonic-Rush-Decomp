/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_020405e0();
extern u8 data_022c198c[];
extern u8 data_022c19a0[];
extern u8 data_022c1a00[];
void func_020402b0();
s32 func_02013064();
s32 func_020405bc();

s32 func_02040304(void) {
    func_020405e0(3);
    s32 t0 = ((s32 (*)())*(u32 *)data_022c198c)(data_022c19a0, data_022c1a00, *(u32 *)data_022c198c);
    s32 t1 = func_02013064((u32)func_020402b0, t0, data_022c19a0);
    if (t1 == 2) {
        return 1;
    }
    func_020405bc(t1);
    func_020405e0(9);
    return 0;
}

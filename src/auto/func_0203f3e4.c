/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_020405e0();
extern u8 data_022c1994[];
extern u8 data_022c19a0[];
extern u8 data_022c1ae0[];
void func_0203f370();
s32 func_02013064();
s32 func_020405bc();

s32 func_0203f3e4(void) {
    func_020405e0(3);
    s32 t0 = ((s32 (*)())*(u32 *)data_022c1994)(data_022c19a0, data_022c1ae0, *(u32 *)data_022c1994);
    s32 t1 = func_02013064((u32)func_0203f370, t0, data_022c19a0);
    if (t1 == 2) {
        return 1;
    }
    func_020405bc(t1);
    func_020405e0(9);
    return 0;
}

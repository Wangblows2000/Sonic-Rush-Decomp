/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c1964[];
extern u8 data_022c1990[];
void func_0203ee8c();
extern u8 data_022c1fc0[];
extern u8 data_022c1da0[];
s32 func_02011dcc();
s32 func_020405bc();

s32 func_0203f070(void) {
    s32 t0 = func_02011dcc((u32)func_0203ee8c, data_022c1fc0, ((u32)(*(u32 *)data_022c1990 << 0x10) >> 0x10), data_022c1da0, ((u32)(*(u32 *)data_022c1964 << 0x10) >> 0x10), 1, 0, 0, 0, 0, 0);
    if (t0 == 2) {
        return 1;
    }
    func_020405bc(t0);
    return 0;
}

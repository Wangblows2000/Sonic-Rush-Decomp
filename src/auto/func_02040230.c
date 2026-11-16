/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c1980[];
void func_02040048();
s32 func_02011904();
s32 func_020405bc();
extern u8 data_022c195c[];
extern u8 data_022c1944[];

s32 func_02040230(void) {
    s32 t0 = (*(u32 *)data_022c1980 - 0x4);
    if ((u32)t0 <= (u32)2) {
        return 1;
    }
    s32 t1 = func_02011904((u32)func_02040048);
    if (t1 != 2) {
        func_020405bc();
        return 0;
    }
    *(u16 *)data_022c195c = 0;
    *(u16 *)data_022c1944 = 1;
    return 1;
}

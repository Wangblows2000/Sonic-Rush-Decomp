/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02042ab8();
extern u8 data_0207ad9c[];
extern u8 data_022c4638[];

void func_02050900(s32 p0) {
    func_02042ab8();
    *(u16 *)data_0207ad9c = 4;
    if ((s32)p0 == *(u32 *)data_022c4638) {
        *(u32 *)data_022c4638 = 0;
    }
}

#pragma thumb reset

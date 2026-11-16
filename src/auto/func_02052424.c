/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022c4560[];
s32 func_0205515c();

void func_02052424(s32 p0, s32 p1) {
    *(u32 *)((u8 *)data_022c4560 + 0x10) = 2;
    s32 t0 = (*(u32 *)((u8 *)data_022c4560 + 0x18) & ~0x20);
    *(u32 *)((u8 *)data_022c4560 + 0x18) = t0;
    *(u32 *)((u8 *)data_022c4560 + 0x18) = (t0 & ~0x10);
    if ((s32)p0 >= 0) {
        *(u8 *)((u8 *)data_022c4560 + 0x1c) = p0;
    }
    s32 v1 = (s32)p0;
    if ((s32)p1 >= 0) {
        *(u8 *)((u8 *)data_022c4560 + 0x1d) = p1;
        v1 = (s32)data_022c4560;
    }
    func_0205515c(v1, p1);
}

#pragma thumb reset

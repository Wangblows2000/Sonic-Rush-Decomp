/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022c465c[];
extern u8 data_0225044c[];
s32 func_0201be98();

void func_02051e90(s32 p0) {
    s32 t0 = *(u32 *)data_022c465c;
    if (t0 != 0) {
        s32 t1 = *(u8 *)data_0225044c;
        if ((s32)p0 != t1) {
            *(u8 *)data_0225044c = p0;
            func_0201be98((t0 + 0x14), p0, data_0225044c, t1);
        }
    }
}

#pragma thumb reset

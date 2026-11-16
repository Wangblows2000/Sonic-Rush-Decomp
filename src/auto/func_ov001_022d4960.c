/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b65b4[];
extern u8 data_022503e0[];

s32 func_ov001_022d4960(void) {
    if ((*(u32 *)((u8 *)data_022b65b4 + 0x10) & 0x40) != 0) {
        s32 t0 = *(u16 *)((u8 *)data_022b65b4 + 0x8);
        s32 t1 = *(u16 *)((u8 *)data_022b65b4 + 0xa);
        if (t0 >= 0x28) {
            if (t0 < 0xd8) {
                if (t1 >= 0x50) {
                    if (t1 < 0x6a) {
                        return 1;
                    }
                }
            }
        }
    }
    if ((*(u16 *)((u8 *)data_022503e0 + 0x4) & 8) != 0) {
        return 1;
    }
    return 0;
}

#pragma thumb reset

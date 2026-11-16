/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_0206047c();
s32 func_02060e70();
extern u8 data_02076c54[];
s32 func_020367b4();

void func_020607d4(u8 *p0) {
    s32 t0 = func_0206047c(*(u32 *)((u8 *)p0 + 0x24));
    s32 t1 = ((s32)(*(u32 *)((u8 *)t0 + 0x18) << 0x10) >> 0x10);
    s32 t2 = func_02060e70(0);
    if ((t2 & 0x10) == 0) {
        if (t1 >= 0) {
            if (t1 < 0xa) {
                if (t1 != 0) {
                    if (t1 != 1) {
                        if (t1 != 2) {
                            func_020367b4(((u8 *)p0 + 0x2c), *(s16 *)((u8 *)data_02076c54 + (t1 << 1)));
                        }
                    }
                }
            }
        }
    }
}

#pragma thumb reset

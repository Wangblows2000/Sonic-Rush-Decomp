/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_ov001_022ddd94();
s32 func_020371c8();
s32 func_ov001_022ddd70();
extern u8 data_02087ccc[];
extern u8 data_02087cb8[];
s32 func_ov001_022d731c();
s32 func_ov001_022d717c();

void func_ov001_022d82dc(void) {
    s32 t0 = *(u32 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t1 = func_ov001_022ddd94(*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10));
    if (t1 != 0) {
        func_020371c8(*(u32 *)data_022b4574);
        s32 t2 = func_ov001_022ddd70();
        if (t2 != 0) {
            *(u32 *)data_02087ccc = 0;
            s32 t3 = (*(u32 *)data_02087cb8 & ~0x10);
            *(u32 *)data_02087cb8 = t3;
            func_ov001_022d731c(t0, data_02087cb8, t3);
            func_ov001_022d717c(t0, 1, 0);
            return;
        }
        *(u32 *)t0 = 3;
    }
}

#pragma thumb reset

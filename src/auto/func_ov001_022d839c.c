/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_ov001_022dddbc();
s32 func_020371c8();
s32 func_ov001_022ddd88();
extern u8 data_02087ccc[];
extern u8 data_02087cb8[];
s32 func_ov001_022d731c();
s32 func_ov001_022d717c();

void func_ov001_022d839c(void) {
    s32 t0 = *(u32 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t1 = func_ov001_022dddbc(*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10));
    if (t1 != 0) {
        func_020371c8(*(u32 *)data_022b4574);
        s32 t2 = func_ov001_022ddd88();
        if (t2 != 0) {
            *(u32 *)data_02087ccc = 0;
            s32 t3 = (*(u32 *)data_02087cb8 & ~0x10);
            *(u32 *)data_02087cb8 = t3;
            func_ov001_022d731c(t0, data_02087cb8, t3);
            func_ov001_022d717c(t0, 1, 1);
            return;
        }
        *(u32 *)t0 = 4;
    }
}

#pragma thumb reset

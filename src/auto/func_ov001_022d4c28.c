/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_ov001_022d42ec();
s32 func_ov001_022d4ed4();
s32 func_ov001_022d4e4c();
s32 func_ov001_022d4d94();
s32 func_02037d60();
extern u8 data_0208926c[];
s32 func_ov001_022d48ec();
s32 func_ov001_022d46f4();
s32 func_ov001_022d44d4();
void func_ov001_022d4af8();

void func_ov001_022d4c28(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    *(u32 *)t0 = (*(u32 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x1);
    s32 t1 = *(u32 *)t0;
    if (t1 >= 2) {
        func_ov001_022d42ec(t1);
        func_ov001_022d4ed4(t0);
        func_ov001_022d4e4c(t0);
        func_ov001_022d4d94(t0);
        func_02037d60();
        func_ov001_022d48ec(*(s16 *)((u8 *)data_0208926c + 8), 0, 8, 0);
        func_ov001_022d46f4((t0 + 0xdc), 1, 0x11, 4, 0x1e0);
        func_ov001_022d46f4((t0 + 0x11c), 1, 0x11, 4, 0x1e0);
        func_ov001_022d44d4(t0);
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_ov001_022d4af8;
    }
}

#pragma thumb reset

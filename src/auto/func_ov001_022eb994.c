/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_ov001_022e9ba8();
s32 func_ov001_022ea3e0();
s32 func_ov001_022ea41c();
s32 func_ov001_022ea7c4();
s32 func_ov001_022ea6e4();
s32 func_02052104();
void func_ov001_022ea894();

void func_ov001_022eb994(s32 p0) {
    s32 v0 = (s32)p0;
    if ((s32)p0 == 0) {
        v0 = *(u32 *)data_022b4574;
    }
    s32 t1 = *(u32 *)((u8 *)v0 + 0x10);
    *(u32 *)t1 = 0;
    s32 v2;
    if ((u32)*(u32 *)((u8 *)t1 + 0x4) <= (u32)1) {
        v2 = 0x10;
    } else {
        v2 = -16;
    }
    if (*(u32 *)(t1 + 0xe4) != 0) {
        func_ov001_022e9ba8(0, v2, 8, 8);
        func_ov001_022ea3e0(t1);
    } else {
        func_ov001_022e9ba8(0, v2, 8, 0);
        func_ov001_022ea41c(t1);
    }
    func_ov001_022ea7c4(t1);
    func_ov001_022ea6e4(t1);
    func_02052104(1);
    *(u32 *)((u8 *)v0 + 0x8) = (u32)func_ov001_022ea894;
}

#pragma thumb reset

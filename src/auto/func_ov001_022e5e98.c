/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_ov001_022e2ee8();
s32 func_ov001_022e3180();
s32 func_02051e7c();
void func_ov001_022e7504();

void func_ov001_022e5e98(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t1 = func_ov001_022e2ee8(*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x8));
    if (t1 != 0) {
        func_ov001_022e3180(*(u32 *)((u8 *)*(u32 *)(t0 + 0xa4) + 0x1cc4), 2);
    } else {
        func_ov001_022e3180(*(u32 *)((u8 *)*(u32 *)(t0 + 0xa4) + 0x1cc4), 0);
    }
    *(u32 *)(t0 + 0x84) = 0;
    *(u32 *)(t0 + 0x88) = 0;
    *(u32 *)(t0 + 0x8c) = 0;
    func_02051e7c(0xf, 0);
    *(u32 *)((u8 *)t0 + 0x74) = 0;
    *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_ov001_022e7504;
}

#pragma thumb reset

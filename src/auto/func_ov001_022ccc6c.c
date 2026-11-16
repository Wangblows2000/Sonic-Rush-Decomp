/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_ov001_022cc17c();
s32 func_ov001_022cbf30();
s32 func_020371c8();

void func_ov001_022ccc6c(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t1 = func_ov001_022cc17c(*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10));
    func_ov001_022cbf30(t0);
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    if (t1 != 0) {
        if ((u32)*(u32 *)t0 >= (u32)0x2e) {
            *(u32 *)*(u32 *)(t0 + 0x3ac) = 0xa;
            *(u32 *)((u8 *)*(u32 *)(t0 + 0x3ac) + 0x4) = 0;
            func_020371c8(*(u32 *)data_022b4574, 0, 0xa);
        }
    }
}

#pragma thumb reset

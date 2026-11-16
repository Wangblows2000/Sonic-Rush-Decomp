/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_ov001_022e6dec();
s32 func_ov001_022e6870();

void func_ov001_022e6e9c(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    func_ov001_022e6dec(*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10));
    *(u32 *)((u8 *)t0 + 0x74) = (*(u32 *)((u8 *)t0 + 0x74) + 0x1);
    s32 t1 = *(u32 *)((u8 *)t0 + 0x74);
    if ((u32)t1 >= (u32)*(u32 *)(t0 + 0x80)) {
        *(u32 *)((u8 *)t0 + 0x74) = 0;
        func_ov001_022e6870(0, t1);
    }
}

#pragma thumb reset

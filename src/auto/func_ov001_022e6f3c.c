/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_ov001_022e213c();
s32 func_02051e7c();
s32 func_ov001_022e6dec();

void func_ov001_022e6f3c(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    if (*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x74) == 0xa4) {
        func_ov001_022e213c(0, 0x10, 0x10, 0, 1, 1);
        func_02051e7c(0xf);
    }
    func_ov001_022e6dec(t0);
    *(u32 *)((u8 *)t0 + 0x74) = (*(u32 *)((u8 *)t0 + 0x74) + 0x1);
    if ((u32)*(u32 *)((u8 *)t0 + 0x74) >= (u32)0xb4) {
        *(u32 *)((u8 *)*(u32 *)(t0 + 0xa4) + 0x1cc8) = 3;
        *(u32 *)((u8 *)*(u32 *)(t0 + 0xa4) + 0x1ccc) = -1;
        *(u32 *)((u8 *)*(u32 *)(t0 + 0xa4) + 0x1cd0) = -1;
        *(u32 *)*(u32 *)(t0 + 0xa4) = 1;
    }
}

#pragma thumb reset

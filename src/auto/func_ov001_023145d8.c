/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f6a0[];
s32 func_0202a20c();

void func_ov001_023145d8(void) {
    s32 t0 = *(u32 *)data_ov001_0231f6a0;
    func_0202a20c(*(u32 *)data_ov001_0231f6a0, 0xff, 0x100);
    *(u16 *)(t0 + 0x100) = 0;
    *(u16 *)((u8 *)(t0 + 0x100) + 0x2) = 0;
    *(u16 *)((u8 *)(t0 + 0x100) + 0x4) = 0;
}

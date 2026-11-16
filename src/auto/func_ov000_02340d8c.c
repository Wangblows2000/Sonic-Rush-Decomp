/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236aad8[];
s32 func_ov000_0233f8a0();
s32 func_ov000_0233f6f8();

void func_ov000_02340d8c(u8 *p0) {
    s32 t0 = (*(u32 *)p0 + 0x1);
    *(u32 *)p0 = t0;
    if ((*(u32 *)((u8 *)data_ov000_0236aad8 + 0xc8) & 1) != 0) {
        func_ov000_0233f8a0(p0, 0, t0);
        func_ov000_0233f6f8(p0);
        return;
    }
    func_ov000_0233f8a0(p0, 0);
    func_ov000_0233f8a0(p0, 1);
}

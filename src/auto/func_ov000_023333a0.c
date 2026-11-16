/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c4560[];
s32 func_ov000_02333098();

void func_ov000_023333a0(u8 *p0) {
    if (*(u8 *)((u8 *)p0 + 0x325) != 0) {
        return;
    }
    if ((u32)*(u8 *)((u8 *)data_022c4560 + 0x1c) >= (u32)9) {
        return;
    }
    if ((u32)*(u8 *)((u8 *)data_022c4560 + 0x1d) >= (u32)2) {
        return;
    }
    if ((*(u32 *)((u8 *)p0 + 0x330) & 0x1000) != 0) {
        func_ov000_02333098(p0, -8, 0);
        return;
    }
    func_ov000_02333098(p0, -8, 8);
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236a97c[];
s32 func_ov000_02336cec();

void func_ov000_0232afb4(s32 p0, s32 p1) {
    s32 t0 = *(u32 *)data_ov000_0236a97c;
    if (t0 == 0) {
        return;
    }
    func_ov000_02336cec(((*(u32 *)((u8 *)t0 + 0x10) + 0x134) + ((s32)p0 << 2)), p1, (*(u32 *)((u8 *)t0 + 0x10) + 0x134));
}

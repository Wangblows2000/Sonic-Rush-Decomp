/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236ac18[];
s32 func_ov000_0233d0fc();
s32 func_020073b8();

void func_01fff760(s32 p0) {
    s32 t0 = *(u32 *)data_ov000_0236ac18;
    if (t0 == 0) {
        return;
    }
    func_ov000_0233d0fc(t0, p0);
    func_020073b8(((s32)p0 * 0x28 + (t0 + 0xc)), 0xc8, 5, (u32)func_01fff760, p0);
}

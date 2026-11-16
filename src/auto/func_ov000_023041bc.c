/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_023425f4();
s32 func_ov000_02304064();
s32 func_ov000_02304184();

void func_ov000_023041bc(u8 *p0) {
    func_ov000_023425f4(*(u32 *)((u8 *)p0 + 0x10), *(u32 *)((u8 *)p0 + 0x14));
    s32 t0 = func_ov000_02304064(p0);
    if (t0 == 0) {
        return;
    }
    func_ov000_02304184(p0);
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_02347f94();

void func_ov000_02302350(u8 *p0, u8 *p1) {
    u32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    u32 t1 = *(u32 *)((u8 *)p1 + 0x18);
    if (t0 == 0) {
        return;
    }
    s32 v2 = (s32)p1;
    if (t1 == 0) {
        u8 t3 = *(u8 *)t1;
        if (t3 != 1) {
            return;
        }
        v2 = t3;
    }
    func_ov000_02347f94(t1, v2);
    u32 t4 = *(u32 *)((u8 *)t0 + 0x54);
    *(u32 *)((u8 *)t0 + 0x54) = (t4 | 4);
}

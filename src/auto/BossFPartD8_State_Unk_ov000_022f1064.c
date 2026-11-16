/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022f898c();
s32 func_ov000_022f3f8c();
s32 func_ov000_022f88dc();

void BossFPartD8_State_Unk_ov000_022f1064(u8 *p0) {
    s32 t0 = (*(u8 *)((u8 *)p0 + 0x57c) & 2);
    if (t0 == 0) {
        return;
    }
    s32 t1 = *(u32 *)((u8 *)p0 + 0x434);
    if (t1 != 0) {
        func_ov000_022f898c(t1, t0);
        return;
    }
    func_ov000_022f3f8c();
    func_ov000_022f88dc();
}

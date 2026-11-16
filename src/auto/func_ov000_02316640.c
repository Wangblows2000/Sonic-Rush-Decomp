/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236a244[];
s32 func_020686dc();
s32 func_02039bb0();
s32 func_ov000_022fb9c8();

void func_ov000_02316640(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    func_020686dc(data_ov000_0236a244);
    s32 t1 = *(u32 *)((u8 *)t0 + 0x240);
    if (t1 != 0) {
        func_02039bb0(0, t1);
    }
    s32 t2 = *(u32 *)((u8 *)t0 + 0x288);
    if (t2 != 0) {
        func_02039bb0(1, t2);
    }
    func_ov000_022fb9c8(p0);
}

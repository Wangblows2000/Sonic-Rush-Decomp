/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_02314b34();

void RopeCSpring_State_Idle(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x88);
    if (t0 == 0) {
        return;
    }
    s32 t1 = (*(u32 *)((u8 *)t0 + 0x54) & 2);
    if (t1 == 0) {
        return;
    }
    func_ov000_02314b34(p0, t1);
}

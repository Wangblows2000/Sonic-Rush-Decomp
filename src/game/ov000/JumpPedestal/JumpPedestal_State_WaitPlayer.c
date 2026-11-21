/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 JumpPedestal_CheckLaunch();

void JumpPedestal_State_WaitPlayer(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x1bc);
    if (t0 == 0) {
        return;
    }
    s32 t1 = *(u8 *)t0;
    if (t1 != 1) {
        return;
    }
    JumpPedestal_CheckLaunch(p0, t0, t1);
}

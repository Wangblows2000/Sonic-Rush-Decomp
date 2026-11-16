/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 SpecialStagePlayer_IsGrounded();
s32 SpecialStagePlayer_ChangeState();
s32 SpecialStagePlayer_CheckTouchTarget();

void func_ov001_02311abc(u8 *p0) {
    s32 t0 = SpecialStagePlayer_IsGrounded();
    if (t0 != 0) {
        *(u32 *)((u8 *)p0 + 0x18) = 0;
        SpecialStagePlayer_ChangeState(p0, 5, 0);
        return;
    }
    SpecialStagePlayer_CheckTouchTarget(p0);
}

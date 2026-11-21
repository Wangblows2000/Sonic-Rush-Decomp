/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 SpecialStagePlayer_IsGrounded();
s32 SpecialStagePlayer_ChangeState();

void SpecialStagePlayer_State_EdgeJump(u8 *p0) {
    s32 t0 = SpecialStagePlayer_IsGrounded();
    if (t0 == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x18) = 0;
    *(u32 *)((u8 *)p0 + 0x48) = 0;
    *(u32 *)((u8 *)p0 + 0x24) = 0;
    SpecialStagePlayer_ChangeState(p0, 7, 0);
}

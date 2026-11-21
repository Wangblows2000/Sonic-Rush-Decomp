/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 SpecialStagePlayer_ChangeState();

void SpecialStagePlayer_State_Celebrate(u8 *p0) {
    if ((*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0xac) + 0x10c) & 2) == 0) {
        return;
    }
    SpecialStagePlayer_ChangeState(p0, 0x15);
}

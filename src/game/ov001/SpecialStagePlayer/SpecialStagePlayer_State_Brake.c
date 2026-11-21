/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 SpecialStagePlayer_SlowDownMap();
s32 SpecialStageMap_GetScrollSpeed();
s32 SpecialStagePlayer_ChangeState();

void SpecialStagePlayer_State_Brake(s32 p0) {
    SpecialStagePlayer_SlowDownMap();
    s32 t0 = SpecialStageMap_GetScrollSpeed();
    if (t0 != 0) {
        return;
    }
    SpecialStagePlayer_ChangeState(p0, 0x1a);
}

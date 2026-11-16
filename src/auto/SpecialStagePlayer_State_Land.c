/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 SpecialStagePlayer_ChangeState();
s32 SpecialStagePlayer_UpdateAngleFromX();
s32 SpecialStageMap_SetScrollSpeed();

void SpecialStagePlayer_State_Land(u8 *p0) {
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x64) | 0x2000);
    *(u16 *)((u8 *)p0 + 0x64) = t0;
    SpecialStagePlayer_ChangeState(p0, 2, t0);
    SpecialStagePlayer_UpdateAngleFromX(p0);
    s32 t1 = *(u16 *)((u8 *)p0 + 0x16);
    *(u32 *)((u8 *)p0 + 0x134) = t1;
    SpecialStageMap_SetScrollSpeed(0x3000, t1);
}

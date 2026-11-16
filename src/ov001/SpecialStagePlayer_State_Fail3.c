#include <nitro/types.h>

s32 SpecialStageMap_GetScrollSpeed(void);
s32 FX_Div(s32 a, s32 b);
s32 SpecialStagePlayer_GetSpeedAnimRate(s32 a);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void SpecialStagePlayer_ChangeState(void *p, s32 v);

void SpecialStagePlayer_State_Fail3(u8 *p) {
    s32 v = SpecialStageMap_GetScrollSpeed();
    FX_Div(v, 0x3000);
    *(s32 *)(*(u8 **)(p + 0xac) + 0x114) = SpecialStagePlayer_GetSpeedAnimRate(v);
    if (v >= 0xc00) {
        return;
    }
    (*(u8 **)(p + 0xac))[0x10c] &= ~4;
    Sound_PlaySeqArc(0, 0x10, 0x10);
    SpecialStagePlayer_ChangeState(p, 0x17);
}

#include <nitro/types.h>

BOOL SpecialStagePlayer_IsGrounded(void *p);
void SpecialStagePlayer_ChangeState(void *p, s32 v);

void SpecialStagePlayer_State_TrickEnd(u8 *p) {
    if (*(s32 *)(p + 0x15c) < 2) {
        u16 old = *(u16 *)(p + 0x12);
        *(u16 *)(p + 0x12) = old + 0x780;
        if (old < 0x8000 && *(u16 *)(p + 0x12) >= 0x8000) {
            *(s32 *)(p + 0x15c) += 1;
            if (*(s32 *)(p + 0x15c) >= 2) {
                *(u16 *)(p + 0x12) = 0x8000;
            }
        }
    }
    if (SpecialStagePlayer_IsGrounded(p)) {
        SpecialStagePlayer_ChangeState(p, 5);
    }
}

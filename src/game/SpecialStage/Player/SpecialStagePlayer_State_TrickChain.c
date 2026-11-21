#include <nitro/types.h>

extern s32 data_ov001_02317924[][13];
void SpecialStagePlayer_Action_StartTrick(void *p, s32 a);

void SpecialStagePlayer_State_TrickChain(u8 *p) {
    if (*(s32 *)(p + 0x15c) < 1) {
        u16 old = *(u16 *)(p + 0x12);
        *(u16 *)(p + 0x12) = old + 0xc00;
        if (old > 0x8000 && *(u16 *)(p + 0x12) <= 0x8000) {
            *(s32 *)(p + 0x15c) += 1;
            if (*(s32 *)(p + 0x15c) >= 1) {
                *(u16 *)(p + 0x12) = 0;
            }
        }
    }
    if (*(s32 *)(p + 0x48) < -data_ov001_02317924[*(s32 *)(p + 0x228)][0]) {
        SpecialStagePlayer_Action_StartTrick(p, *(s16 *)(p + 0x150));
    }
}

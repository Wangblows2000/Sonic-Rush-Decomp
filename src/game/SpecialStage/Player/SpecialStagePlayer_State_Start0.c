#include <nitro/types.h>

void SpecialStagePlayer_ChangeState(void *p, s32 v);

void SpecialStagePlayer_State_Start0(u8 *p) {
    *(s32 *)(*(u8 **)(p + 0xac) + 0x114) += 0x40;
    if (*(s32 *)(*(u8 **)(p + 0xac) + 0x114) < 0x2000) {
        return;
    }
    (*(u8 **)(p + 0xac))[0x10c] &= ~4;
    if ((*(u8 **)(p + 0xac))[0x10c] & 2) {
        SpecialStagePlayer_ChangeState(p, 1);
    }
}

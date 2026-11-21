#include <nitro/types.h>

void SpecialStagePlayer_ChangeState(void *p, s32 v);

void SpecialStagePlayer_State_Start1(u8 *p) {
    *(s32 *)(*(u8 **)(p + 0xac) + 0x114) += 0x40;
    if (*(s32 *)(*(u8 **)(p + 0xac) + 0x114) < 0x3000) {
        return;
    }
    (*(u8 **)(p + 0xac))[0x10c] &= ~4;
    if ((*(u8 **)(p + 0xac))[0x10c] & 2) {
        SpecialStagePlayer_ChangeState(p, 2);
    }
}

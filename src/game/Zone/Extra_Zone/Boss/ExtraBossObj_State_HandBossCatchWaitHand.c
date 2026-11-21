#include <nitro/types.h>

void ExtraBossObj_State_HandBossCatchResetArm(void);

void ExtraBossObj_State_HandBossCatchWaitHand(u8 *p) {
    if (*(u16 *)(*(u8 **)(p + 0x1dc) + 0x14) == 0) {
        *(void (**)(void))(p + 0x74) = ExtraBossObj_State_HandBossCatchResetArm;
    }
}

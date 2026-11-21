#include <nitro/types.h>

void ExtraBossPlayer_UpdateInvincibility(void *p);
void ExtraBossPlayer_CheckActionInput(void *p);

void ExtraBossPlayer_State_Idle(u8 *p) {
    ExtraBossPlayer_UpdateInvincibility(p);
    *(s32 *)(p + 0x50) = -0x1000;
    ExtraBossPlayer_CheckActionInput(p);
}

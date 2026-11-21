#include <nitro/types.h>

void ExtraBossPlayer_UpdateInvincibility(void *p);
BOOL ExtraBossPlayer_CheckActionInput(void *p);
void ExtraBossPlayer_ChangeState(void *p, s32 a);

void ExtraBossPlayer_State_Hurt(u8 *p) {
    ExtraBossPlayer_UpdateInvincibility(p);
    *(s32 *)(p + 0x1c8) -= 1;
    if (*(s32 *)(p + 0x1c8) > 0) {
        return;
    }
    *(s32 *)(p + 0x50) = 0;
    if (ExtraBossPlayer_CheckActionInput(p)) {
        return;
    }
    ExtraBossPlayer_ChangeState(p, 1);
    *(u32 *)(p + 0x14) &= ~0x700;
}

#include <nitro/types.h>

void ExtraBossPlayer_UpdateInvincibility(void *p);
BOOL ExtraBossPlayer_CheckActionInput(void *p);
void ExtraBossPlayer_ApplyMoveInput(void *p);
void ExtraBossPlayer_ChangeState(void *p, s32 a);

void ExtraBossPlayer_State_Move(u8 *p) {
    ExtraBossPlayer_UpdateInvincibility(p);
    if (ExtraBossPlayer_CheckActionInput(p)) {
        return;
    }
    ExtraBossPlayer_ApplyMoveInput(p);
    if (*(s32 *)(p + 0x5c) != 0) {
        return;
    }
    if (*(s32 *)(p + 0x60) != 0) {
        return;
    }
    if (*(s32 *)(p + 0x50) != 0) {
        return;
    }
    if (*(s32 *)(p + 0x54) != 0) {
        return;
    }
    ExtraBossPlayer_ChangeState(p, 1);
    *(u32 *)(p + 0x14) &= ~0x700;
}

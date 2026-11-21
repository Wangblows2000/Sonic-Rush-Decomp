#include <nitro/types.h>

void ExtraBossPlayer_UpdateInvincibility(void *p);
BOOL ExtraBossPlayer_CheckActionInput(void *p);
void ExtraBossPlayer_ChangeState(void *p, s32 a);

void ExtraBossPlayer_State_SpecialEnd(u8 *p) {
    u8 *q = *(u8 **)(p + 0x84);
    ExtraBossPlayer_UpdateInvincibility(p);
    if (!(q[0x10c] & 2)) {
        return;
    }
    if (ExtraBossPlayer_CheckActionInput(p)) {
        return;
    }
    ExtraBossPlayer_ChangeState(p, 1);
    *(u32 *)(p + 0x14) &= ~0x700;
}

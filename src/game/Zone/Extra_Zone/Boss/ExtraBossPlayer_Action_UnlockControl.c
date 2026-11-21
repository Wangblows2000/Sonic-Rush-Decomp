#include <nitro/types.h>

void ExtraBossPlayer_ChangeState(void *p, s32 a);

void ExtraBossPlayer_Action_UnlockControl(u8 *p) {
    ExtraBossPlayer_ChangeState(p, 1);
    *(u32 *)(p + 0x14) &= ~0x1000;
}

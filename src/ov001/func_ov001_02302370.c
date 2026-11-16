#include <nitro/types.h>

u8 *ExtraBoss_GetPlayer(void);
void ExtraBossPlayer_ChangeState(void *p, s32 a);

void func_ov001_02302370(void) {
    u8 *p = ExtraBoss_GetPlayer();
    ExtraBossPlayer_ChangeState(p, 7);
    *(u32 *)(p + 0x14) |= 0x1000;
}

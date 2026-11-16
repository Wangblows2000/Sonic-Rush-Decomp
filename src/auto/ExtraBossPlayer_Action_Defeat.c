/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_GetPlayer();
s32 ExtraBossPlayer_ChangeState();

void ExtraBossPlayer_Action_Defeat(void) {
    s32 t0 = ExtraBoss_GetPlayer();
    s32 t1 = *(u32 *)((u8 *)t0 + 0x1dc);
    ExtraBossPlayer_ChangeState(t0, 6);
    *(u32 *)((u8 *)t0 + 0x14) = (*(u32 *)((u8 *)t0 + 0x14) & 0xfffdf8ff);
    *(u32 *)t1 = (*(u32 *)t1 & ~1);
    *(u32 *)((u8 *)t0 + 0x14) = (*(u32 *)((u8 *)t0 + 0x14) | 0x1000);
    *(u32 *)((u8 *)t0 + 0xa0) = 0;
}

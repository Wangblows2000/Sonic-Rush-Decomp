/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void Boss1Neck_State_MissileIntroAnim();

void Boss1Neck_State_MissileWaitStart(u8 *p0) {
    if ((*(u8 *)((u8 *)p0 + 0x334) & 2) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Boss1Neck_State_MissileIntroAnim;
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x68))(p0, *(u32 *)((u8 *)p0 + 0x68));
}

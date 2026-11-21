/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss1[];
s32 Sound_PlayVoice();
void Boss1Neck_State_MissileRetractAnim();

void Boss1Neck_State_MissileWaitEndAnim(u8 *p0) {
    u8 t0 = *(u8 *)((u8 *)p0 + 0x334);
    u32 t1 = *(u32 *)gBoss1;
    if ((t0 & 2) == 0) {
        return;
    }
    u32 t2 = *(u32 *)((u8 *)t1 + 0x20c);
    *(u32 *)((u8 *)t1 + 0x20c) = (t2 & ~1);
    Sound_PlayVoice(0, 0, 0xea, (t2 & ~1));
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Boss1Neck_State_MissileRetractAnim;
}

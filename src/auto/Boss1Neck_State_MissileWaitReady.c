/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Sound_PlayVoice();
void Boss1Neck_State_MissileSelectPattern();

void Boss1Neck_State_MissileWaitReady(u8 *p0) {
    if ((*(u8 *)((u8 *)p0 + 0x334) & 2) == 0) {
        return;
    }
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x5c) | 0x20);
    *(u16 *)((u8 *)p0 + 0x5c) = t0;
    Sound_PlayVoice(0, 0, 0xd1, t0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Boss1Neck_State_MissileSelectPattern;
}

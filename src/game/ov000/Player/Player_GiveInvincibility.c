/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Sound_PlaySeqArcEx();
s32 StageJingle_Create();
s32 EffInvincibility_Create();

void Player_GiveInvincibility(s32 p0) {
    Sound_PlaySeqArcEx(0, 0, 0x3d);
    StageJingle_Create(p0, 0x16);
    *(u16 *)((u8 *)p0 + 0x3ca) = 0;
    *(u16 *)((u8 *)p0 + 0x3b4) = 0x3e7;
    EffInvincibility_Create(p0, ((u8 *)p0 + 0x300), 0x3e7);
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Sound_PlaySeqArcEx();
extern u8 gPlayers[];
s32 Sound_SetPositional();
s32 ScreenShake_Start();
s32 ScreenFlash_Create();
s32 EffZakoBomb_Create();
s32 AttackHitbox_Create();
void KuragenBomb_State_Exploded();

void KuragenBomb_Action_Explode(u8 *p0) {
    if ((*(u32 *)((u8 *)p0 + 0x54) & 4) != 0) {
        return;
    }
    Sound_PlaySeqArcEx(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x59);
    Sound_SetPositional(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)gPlayers);
    ScreenShake_Start(4);
    ScreenFlash_Create(0);
    EffZakoBomb_Create(p0, 0, -32);
    AttackHitbox_Create(p0, -16, -48, 0x10, 0, 8);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)KuragenBomb_State_Exploded;
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
}

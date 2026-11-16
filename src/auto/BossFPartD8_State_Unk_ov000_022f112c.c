/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossFPartD8_SetAnimation();
extern u8 gPlayers[];
s32 MapObject_Spawn();
s32 Sound_PlayVoice();
void BossFPartD8_State_Unk_ov000_022f10dc();

void BossFPartD8_State_Unk_ov000_022f112c(u8 *p0) {
    BossFPartD8_SetAnimation(p0, 0x2a, 0x1000, 1);
    MapObject_Spawn(0xde, *(u32 *)((u8 *)*(u32 *)gPlayers + 0x10), (*(u32 *)((u8 *)*(u32 *)gPlayers + 0x14) - 0x8000), ((u32)(*(u32 *)((u8 *)p0 + 0x458) << 0x10) >> 0x10), 0, 0, 0, 0, 0);
    Sound_PlayVoice(0, 0, 0xd1);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_Unk_ov000_022f10dc;
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void WaterJet_State_Spout();
s32 NNS_SndPlayerStopSeq();
s32 Sound_PlaySeqArcEx();
extern u8 gPlayers[];
s32 Sound_SetPositional();

void WaterJet_Action_Spout(u8 *p0) {
    StageObj_SetAnimation(p0, 1);
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x16a) & ~0x80);
    *(u16 *)((u8 *)p0 + 0x16a) = t0;
    *(u32 *)((u8 *)p0 + 0xc) = 0x28;
    *(u32 *)((u8 *)p0 + 0x20c) = (*(u32 *)((u8 *)p0 + 0x20c) | 1);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)WaterJet_State_Spout;
    NNS_SndPlayerStopSeq(*(u32 *)((u8 *)p0 + 0x1dc), 0, (u32)WaterJet_State_Spout, t0);
    Sound_PlaySeqArcEx(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x57);
    Sound_SetPositional(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)gPlayers);
}

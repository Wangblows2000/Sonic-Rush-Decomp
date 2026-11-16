/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void WaterJet_State_Prepare();
s32 NNS_SndPlayerStopSeq();
s32 Sound_PlaySeqArcEx();
extern u8 gPlayers[];
s32 Sound_SetPositional();

void WaterJet_Action_Prepare(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~0x10);
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u16 *)((u8 *)p0 + 0x5c) = t0;
    *(u32 *)((u8 *)p0 + 0xc) = 0x50;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)WaterJet_State_Prepare;
    NNS_SndPlayerStopSeq(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x50, t0);
    Sound_PlaySeqArcEx(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x56);
    Sound_SetPositional(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)gPlayers);
}

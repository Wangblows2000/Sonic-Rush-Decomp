/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Piston_UpdateOscillation();
s32 Piston_DrawShaft();
s32 NNS_SndPlayerCountPlayingSeqBySeqArcIdx();
s32 Sound_PlaySeqArcBankPrio();
extern u8 gPlayers[];
s32 Sound_SetPositional();

void Piston_State_Move(u8 *p0) {
    Piston_UpdateOscillation();
    Piston_DrawShaft(p0);
    s32 t0 = NNS_SndPlayerCountPlayingSeqBySeqArcIdx(0, 0x67);
    if (t0 == 0) {
        Sound_PlaySeqArcBankPrio(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x67, 0x20, -1);
    }
    Sound_SetPositional(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)gPlayers);
}

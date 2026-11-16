/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_023126e8();
s32 func_ov000_02312634();
s32 NNS_SndPlayerCountPlayingSeqByPlayerNo();
s32 Sound_PlaySeqArcBankPrio();
extern u8 gPlayers[];
s32 Sound_SetPositional();

void Piston_State_Unk_ov000_023128c4(u8 *p0) {
    func_ov000_023126e8();
    func_ov000_02312634(p0);
    s32 t0 = NNS_SndPlayerCountPlayingSeqByPlayerNo(0, 0x67);
    if (t0 == 0) {
        Sound_PlaySeqArcBankPrio(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x67, 0x20, -1);
    }
    Sound_SetPositional(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)gPlayers);
}

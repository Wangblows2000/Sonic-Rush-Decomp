#include <ov000_c/stageobj.h>

void ScreenShake_Start(s32 a);
UnkStruct_StageObj *EffZakoDebris_Create(UnkStruct_StageObj *w, s16 a, s16 b, s16 c, s16 d, u8 e);
void EffZakoBombPuff_Create(UnkStruct_StageObj *w, s16 x, s16 y);
void NNS_SndPlayerStopSeq(s32 a, s32 b);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void JetCatapult_State_Fly(void);

void JetCatapult_Action_Launch(UnkStruct_StageObj *w) {
    w->unk68 = JetCatapult_State_Fly;
    w->unk58 |= 0x100;
    w->unk58 &= ~1;
    w->unk58 |= 0x10;
    w->unk58 &= ~0x40;
    w->unk5C |= 0x100;
    w->unk1B8 = NULL;
    w->unk30 = -0x280;
    w->unk2E = w->unk34 >> 1;
    w->unk34 = 0;
    ScreenShake_Start(4);
    w->unk62 = 0x44;
    EffZakoDebris_Create(w, 0, 0, 0, -0x200, 0);
    EffZakoDebris_Create(w, 0, 0, 0, -0x200, 1);
    EffZakoBombPuff_Create(w, 0x14, 0);
    NNS_SndPlayerStopSeq(w->unk1DC, 0);
    Sound_PlaySeqArcEx(0, 0, 0x64);
}

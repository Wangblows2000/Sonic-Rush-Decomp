#include <game/Stage/stageobj.h>

void GoalRing_Action_Idle(UnkStruct_StageObj *w);
void NNS_SndPlayerStopSeq(void *a, s32 b);
void NNS_SndPlayerSetTempoRatio(void *a, s32 b);
void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);
void GoalRing_CreatePart_ov000_0230a864(UnkStruct_StageObj *par, s16 dx, s16 dy, s16 vx, s16 vy);

void GoalRing_State_Spin(UnkStruct_StageObj *w) {
    w->unk8++;
    if (w->unk8 == 0x30)
        w->unk1F4 |= 1;
    if (w->unk1F4 & 1) {
        w->unkC--;
        if (w->unkC < 0x10)
            w->unkC = 0x10;
        if (w->unkC == 0x10 && (w->unk5C & 8)) {
            GoalRing_Action_Idle(w);
            w->unkC = 0x60;
            StageObj_SetAnimation(w, 7);
            NNS_SndPlayerStopSeq((void *)w->unk1DC, 0);
            NNS_SndPlayerSetTempoRatio((void *)w->unk1DC, 0x100);
            Sound_PlaySeqArcEx(0, 0, 0x46);
            GoalRing_CreatePart_ov000_0230a864(w, 0, 0, 0x1d0, 0);
            GoalRing_CreatePart_ov000_0230a864(w, 0, 0, 0, 0x1d0);
            GoalRing_CreatePart_ov000_0230a864(w, 0, 0, -0x1d0, 0);
            GoalRing_CreatePart_ov000_0230a864(w, 0, 0, 0, -0x1d0);
            GoalRing_CreatePart_ov000_0230a864(w, 0, 0, 0x148, 0x148);
            GoalRing_CreatePart_ov000_0230a864(w, 0, 0, 0x148, -0x148);
            GoalRing_CreatePart_ov000_0230a864(w, 0, 0, -0x148, -0x148);
            GoalRing_CreatePart_ov000_0230a864(w, 0, 0, -0x148, 0x148);
            return;
        }
    }
    w->spr0.unk2A = w->unkC;
    w->spr1.unk2A = w->spr0.unk2A;
    if (w->unkC <= 0x40)
        NNS_SndPlayerSetTempoRatio((void *)w->unk1DC, w->unkC << 2);
    if (!(w->unk8 & 0xf))
        GoalRing_CreatePart_ov000_0230a864(w, 0xf - (Rand() & 0x1f), 0xf - (Rand() & 0x1f), 0, 0);
}

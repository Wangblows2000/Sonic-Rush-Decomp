#include <ov000_c/stageobj.h>

void StageObj_SetAnimation(void *w, s32 a);
void GoalRing_CreatePart_ov000_0230ab48(UnkStruct_StageObj *w);
void GoalRing_State_Spin(void);

void GoalRing_Action_Spin(UnkStruct_StageObj *w, s32 v) {
    s16 a = (v >> 4) < 0 ? -(v >> 4) : (v >> 4);

    if (a <= 0x10)
        a = 0x10;
    if (a >= 0xc0)
        a = 0xc0;
    if (v > 0)
        StageObj_SetAnimation(w, 1);
    else
        StageObj_SetAnimation(w, 4);
    w->unk5C |= 4;
    w->unkC = a;
    w->spr0.unk2A = a;
    w->spr1.unk2A = a;
    w->unk68 = GoalRing_State_Spin;
    GoalRing_CreatePart_ov000_0230ab48(w);
    w->unk8 = 0;
}

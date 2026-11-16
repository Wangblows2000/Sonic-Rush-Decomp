#include <ov000_c/stageobj.h>

void StageObj_SetAnimation(void *w, s32 a);
void JetCatapult_State_Unk_ov000_0230e6e8(void);

void EffJet_State_FollowParent(UnkStruct_StageObj *w) {
    UnkStruct_StageObj *p = w->unk88;

    if (p == NULL)
        return;
    if (p->unk5C & 1)
        w->unk5C |= 1;
    else
        w->unk5C &= ~1;
    w->unk1 = ((UnkStruct_StageObj *)w->unk88)->unk1;
    w->unk10 = ((UnkStruct_StageObj *)w->unk88)->unk10;
    w->unk14 = ((UnkStruct_StageObj *)w->unk88)->unk14;
    if (w->unkC != 0)
        return;
    if (((UnkStruct_StageObj *)w->unk88)->unk68 != JetCatapult_State_Unk_ov000_0230e6e8)
        return;
    w->unkC = 1;
    StageObj_SetAnimation(w, 1);
    w->unk5C |= 4;
}

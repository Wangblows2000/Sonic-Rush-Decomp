#include <ov000_b/stageobj.h>

void Spring_State_Activated(void);

void Spring_Action_Activate(UnkStruct_StageObj *w) {
    StageObj_SetAnimation(w, (u16)(w->unk8 + 1));
    w->unk68 = Spring_State_Activated;
    if (w->unk1E0->unk4 & 4)
        w->unk5C |= 0x20;
    else
        w->unk5C &= ~0x20;
    w->spr0.unk2C &= ~0x60000;
    w->spr1.unk2C &= ~0x60000;
    w->spr0.unk2C |= 0x40000;
    w->spr1.unk2C |= 0x40000;
}

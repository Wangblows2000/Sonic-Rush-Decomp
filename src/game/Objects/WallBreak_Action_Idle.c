#include <game/Stage/stageobj_alt.h>

void WallBreak_State_Idle(void);

void WallBreak_Action_Idle(UnkStruct_StageObj *w) {
    u16 f = w->unk1E0->unk4;

    switch (f & 3) {
    case 0:
        StageObj_SetAnimation(w, 0);
        break;
    case 1:
        StageObj_SetAnimation(w, 1);
        break;
    case 2:
    default:
        StageObj_SetAnimation(w, 2);
        break;
    }
    w->unk68 = WallBreak_State_Idle;
}

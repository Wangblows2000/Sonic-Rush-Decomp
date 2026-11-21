#include <game/Stage/stageobj_alt.h>

void DecAnimal_State_Move(void);

void DecAnimal_Action_Move(UnkStruct_EffObj *w) {
    if (w->unk68 != DecAnimal_State_Move)
        StageObj_SetAnimation(w, 2);
    w->unk5C |= 4;
    w->unk68 = DecAnimal_State_Move;
}

#include <game/Stage/stageobj_alt.h>

void DecAnimal_UpdateMove(UnkStruct_EffObj *w);
void DecAnimal_ChooseNextAction(UnkStruct_EffObj *w);
void DecAnimal_CheckTouch(UnkStruct_EffObj *w);
void DecAnimal_UpdateLeaveTimer(UnkStruct_EffObj *w);

void DecAnimal_State_Move(UnkStruct_EffObj *w) {
    DecAnimal_UpdateMove(w);
    if (w->unkC == 0) {
        if ((w->unk58 & 1) && !(w->unk58 & 0x2000)) {
            DecAnimal_ChooseNextAction(w);
            return;
        }
    } else {
        w->unkC--;
    }
    DecAnimal_CheckTouch(w);
    DecAnimal_UpdateLeaveTimer(w);
}

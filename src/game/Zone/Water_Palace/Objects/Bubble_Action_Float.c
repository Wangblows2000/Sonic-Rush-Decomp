#include <game/Stage/stageobj.h>

void StageObj_SetAnimation(void *w, s32 a);
void Bubble_State_Float(void);

void Bubble_Action_Float(UnkStruct_StageObj *w) {
    switch (w->unk1E0->unk2) {
    case 0xe8:
        break;
    case 0xe9:
        StageObj_SetAnimation(w, 2);
        w->unk30 = -0x101 - (Rand() & 0x7e);
        break;
    case 0xea:
        StageObj_SetAnimation(w, 3);
        w->unk30 = -0x131 - (Rand() & 0x7e);
        break;
    }
    w->unk5C |= 4;
    w->unk58 |= 0x10;
    w->unk68 = Bubble_State_Float;
}

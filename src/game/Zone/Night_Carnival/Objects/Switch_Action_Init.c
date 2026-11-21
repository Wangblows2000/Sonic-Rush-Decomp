#include <game/Stage/stageobj_alt.h>

extern s16 gSwitchTimers[];

void Switch_State_Vertical(void);
void Switch_State_Horizontal(void);

void Switch_Action_Init(UnkStruct_StageObj *w) {
    UnkStruct_ObjParam *p = w->unk1E0;

    if (p->unk2 == 0xbe) {
        if (gSwitchTimers[p->unk6] != 0) {
            w->unkC = 0x200;
            if (w->unk5C & 2)
                w->unk14 -= 0x200;
            else
                w->unk14 += 0x200;
        }
        w->unk68 = Switch_State_Vertical;
        return;
    }
    if (gSwitchTimers[p->unk6] != 0) {
        w->unkC = 0x200;
        if (!(w->unk5C & 1)) {
            w->unk10 += 0x200;
        } else {
            w->unk10 -= 0x200;
            w->unkC = -w->unkC;
        }
    }
    w->unk68 = Switch_State_Horizontal;
}

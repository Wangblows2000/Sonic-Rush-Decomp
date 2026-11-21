#include <game/Menus/menuo.h>

BOOL TimeSelectMenu_IsConfirmInput(UnkStruct_MenuO *w);
BOOL KeyframeAnim_IsFinished(UnkStruct_MenuOPos *p);
void TimeSelectMenu_State_Select(void);

#pragma thumb on

void TimeSelectMenu_State_WaitScroll(void) {
    UnkStruct_MenuO *w = gTaskCurrent->work;

    MenuO_Update(w, 1);
    if (TimeSelectMenu_IsConfirmInput(w)) {
        w->unk18 = 1;
    }
    if (KeyframeAnim_IsFinished(&w->pos[0])) {
        w->unk0 = 0;
        gTaskCurrent->unk8 = TimeSelectMenu_State_Select;
    }
}

#pragma thumb reset

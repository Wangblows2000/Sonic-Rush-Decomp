#include <ov000_d/scene.h>

void PlayerSelectMenu_State_QuitPrompt(void);

#pragma thumb on

void PlayerSelectMenu_Action_OpenQuitPrompt(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;

    if (t == NULL) {
        t = gTaskCurrent;
    }
    w = t->work;
    w->unk0 = 0;
    if (w->unk33C) {
        func_ov000_02357aac(w);
    } else {
        func_ov000_02357ae8(w);
    }
    func_ov000_02357d20(w);
    func_ov000_02357c8c(w);
    MsgBox_Create(9, 3, 1, 0, 1, 0, 0xc, 0x100010);
    w->unk334 = 0;
    t->unk8 = PlayerSelectMenu_State_QuitPrompt;
}

#pragma thumb reset

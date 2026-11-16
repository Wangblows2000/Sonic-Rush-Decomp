#include <ov000_d/scene.h>

void PlayerSelectMenu_SetLocalChoice(UnkStruct_Scene *w, s32 sel);
void PlayerSelectMenu_State_Confirmed(void);

#pragma thumb on

void PlayerSelectMenu_Action_Confirm(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;

    if (t == NULL) {
        t = gTaskCurrent;
    }
    w = t->work;
    w->unk0 = 0;
    w->unk4 = 0;
    PlayerSelectMenu_SetModelSelection(&w->mdl, 1);
    func_ov000_02357b9c(w);
    func_ov000_02357d20(w);
    func_ov000_02357c8c(w);
    func_ov000_02357a30(w, w->unk14);
    if (!(w->unkC != 1 && w->unkC != 2)) {
        PlayerSelectMenu_SetLocalChoice(w, w->unk14);
    }
    if (w->unkC != 1 && w->unkC != 2) {
        switch (w->unk14) {
        case 0:
            SysSound_PlayMenuVoice(2);
            break;
        case 1:
            SysSound_PlayMenuVoice(3);
            break;
        case 2:
            SysSound_PlayMenuVoice(4);
            SysSound_PlayMenuVoiceAlt(5);
            break;
        }
    }
    w->unk334 = 0;
    w->unk8 = 0;
    t->unk8 = PlayerSelectMenu_State_Confirmed;
}

#pragma thumb reset

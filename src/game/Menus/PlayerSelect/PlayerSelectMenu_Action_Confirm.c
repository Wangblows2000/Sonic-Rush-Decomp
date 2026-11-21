#include <game/Menus/scene.h>

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
    PlayerSelectMenu_SetSprite9B4ConfirmAnim(w);
    PlayerSelectMenu_SetSprite60CAnim3(w);
    PlayerSelectMenu_SetSprite654Anim3(w);
    PlayerSelectMenu_SetSprite804ConfirmAnim(w, w->unk14);
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

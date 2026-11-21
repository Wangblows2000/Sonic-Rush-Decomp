#include <game/Menus/scene.h>

void PlayerSelectMenu_State_FadeOutBack(void);

#pragma thumb on

void PlayerSelectMenu_Action_FadeOutBack(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;

    if (t == NULL) {
        t = gTaskCurrent;
    }
    w = t->work;
    w->unk0 = 0;
    if (w->unk33C) {
        PlayerSelectMenu_SetSpriteA8CAnim1(w);
    } else {
        PlayerSelectMenu_SetSpriteA8CAnim0(w);
    }
    PlayerSelectMenu_SetSprite60CAnim3(w);
    PlayerSelectMenu_SetSprite654Anim3(w);
    if (w->unk33C) {
        PlayerSelectMenu_StartBrightnessFade(0, 0x10, 0x10, 0x18);
    } else {
        PlayerSelectMenu_StartBrightnessFade(0, 0x10, 0x10, 0);
        w->unk0 = 0x18;
    }
    w->unk334 = 0;
    t->unk8 = PlayerSelectMenu_State_FadeOutBack;
}

#pragma thumb reset

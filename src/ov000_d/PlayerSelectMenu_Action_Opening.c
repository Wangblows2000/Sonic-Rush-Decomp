#include <ov000_d/scene.h>

void PlayerSelectMenu_Main(void);

#pragma thumb on

void PlayerSelectMenu_Action_Opening(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;

    if (t == NULL) {
        t = gTaskCurrent;
    }
    w = t->work;
    w->unk0 = 0;
    KeyframeAnim_SetAnim(&w->posB68, 0);
    PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, 0x100);
    w->unk330 = -0x100;
    PlayerSelectMenu_SetSprite5C4Anim0(w);
    t->unk8 = PlayerSelectMenu_Main;
}

#pragma thumb reset

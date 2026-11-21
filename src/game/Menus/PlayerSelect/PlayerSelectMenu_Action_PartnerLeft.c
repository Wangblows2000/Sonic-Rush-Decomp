#include <game/Menus/scene.h>

void WirelessSession_Stop(void);
void PlayerSelectMenu_State_PartnerLeft(void);

#pragma thumb on

void PlayerSelectMenu_Action_PartnerLeft(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;

    if (t == NULL) {
        t = gTaskCurrent;
    }
    w = t->work;
    w->unk0 = 0;
    PlayerSelectMenu_SetSprite60CAnim3(w);
    PlayerSelectMenu_SetSprite654Anim3(w);
    Task_DestroyRange(0x4000, 0x4000);
    WirelessSession_Stop();
    w->unk334 = 0;
    t->unk8 = PlayerSelectMenu_State_PartnerLeft;
}

#pragma thumb reset

#include <ov000_d/scene.h>

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
    func_ov000_02357d20(w);
    func_ov000_02357c8c(w);
    Task_DestroyRange(0x4000, 0x4000);
    WirelessSession_Stop();
    w->unk334 = 0;
    t->unk8 = PlayerSelectMenu_State_PartnerLeft;
}

#pragma thumb reset

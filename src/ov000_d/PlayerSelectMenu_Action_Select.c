#include <ov000_d/scene.h>

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
} UnkStruct_023599b0_Sync;

UnkStruct_023599b0_Sync *WirelessSession_GetSendBuffer(void);
void PlayerSelectMenu_State_Select(void);

#pragma thumb on

void PlayerSelectMenu_Action_Select(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;
    UnkStruct_023599b0_Sync *s;

    if (t == NULL) {
        t = gTaskCurrent;
    }
    w = t->work;
    w->unk0 = 0;
    func_ov000_02357b4c(w);
    func_ov000_02357b24(w);
    func_ov000_02357be4(w);
    func_ov000_02357b74(w);
    func_ov000_02357ae8(w);
    func_ov000_02357aa8(w, 0);
    func_ov000_02357aa8(w, 1);
    func_ov000_02357aa8(w, 2);
    func_ov000_02357a6c(w, w->unk14);
    func_ov000_02358180(w);
    s = WirelessSession_GetSendBuffer();
    s->unk0 = w->unk14;
    s->unk2 = 0;
    s->unk1 = 0;
    t->unk8 = PlayerSelectMenu_State_Select;
}

#pragma thumb reset

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
    PlayerSelectMenu_SetSprite69CAnim6(w);
    PlayerSelectMenu_SetSprite6E4Anim7(w);
    PlayerSelectMenu_SetSprite9B4Anim(w);
    PlayerSelectMenu_SetSprite9FCAnim1D(w);
    PlayerSelectMenu_SetSpriteA8CAnim0(w);
    PlayerSelectMenu_Unk_ov000_02357aa8(w, 0);
    PlayerSelectMenu_Unk_ov000_02357aa8(w, 1);
    PlayerSelectMenu_Unk_ov000_02357aa8(w, 2);
    PlayerSelectMenu_SetSprite804SelectAnim(w, w->unk14);
    PlayerSelectMenu_SetSprite5C4Anim1(w);
    s = WirelessSession_GetSendBuffer();
    s->unk0 = w->unk14;
    s->unk2 = 0;
    s->unk1 = 0;
    t->unk8 = PlayerSelectMenu_State_Select;
}

#pragma thumb reset

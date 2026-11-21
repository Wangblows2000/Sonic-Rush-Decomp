#include <game/Menus/scene.h>

typedef struct {
    u8 pad0[3];
    u8 unk3;
} UnkStruct_02359af4_Sync;

UnkStruct_02359af4_Sync *WirelessSession_GetSendBuffer(void);
void GX_SetGraphicsMode(s32 a, s32 b, s32 c);
void PlayerSelectMenu_State_Intro(void);

#pragma thumb on

void PlayerSelectMenu_Action_Intro(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;

    if (t == NULL) {
        t = gTaskCurrent;
    }
    w = t->work;
    w->unk0 = 0;
    w->unk20 = 1;
    w->unk18 = 4;
    KeyframeAnim_SetAnim(&w->pos[6], 9);
    KeyframeAnim_SetAnim(&w->pos[7], 0xb);
    KeyframeAnim_SetAnim(&w->pos[0], 1);
    KeyframeAnim_SetAnim(&w->pos[1], 2);
    KeyframeAnim_SetAnim(&w->pos[2], 3);
    KeyframeAnim_SetAnim(&w->pos[3], 4);
    KeyframeAnim_SetAnim(&w->pos[4], 5);
    KeyframeAnim_SetAnim(&w->pos[5], 7);
    PlayerSelectMenu_SetSprite60CAnim3(w);
    PlayerSelectMenu_SetSprite654Anim3(w);
    PlayerSelectMenu_SetSprite69CAnim6(w);
    PlayerSelectMenu_SetSprite6E4Anim7(w);
    PlayerSelectMenu_SetSprite9B4Anim(w);
    PlayerSelectMenu_SetSpriteA8CAnim0(w);
    PlayerSelectMenu_UpdateCharLayerFlags(w);
    if (w->unk14 != 2) {
        PlayerSelectMenu_SetSprite96CAnim0(w);
    } else {
        PlayerSelectMenu_SetSprite96CAnim1(w);
    }
    GX_SetGraphicsMode(1, 0, 1);
    *(vu16 *)0x400000e = (*(vu16 *)0x400000e & 0x43) | 0x69c;
    w->unk334 = 1;
    w->unk338 = 1;
    t->unk8 = PlayerSelectMenu_State_Intro;
    WirelessSession_GetSendBuffer()->unk3 = 0;
}

#pragma thumb reset

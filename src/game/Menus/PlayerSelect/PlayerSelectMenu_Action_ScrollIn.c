#include <game/Menus/scene.h>

void PlayerSelectMenu_State_ScrollIn(void);

#pragma thumb on

void PlayerSelectMenu_Action_ScrollIn(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;

    if (t == NULL) {
        t = gTaskCurrent;
    }
    w = t->work;
    w->unk0 = 0;
    w->unk1C = w->unk14;
    w->unk14 = w->unk18;
    w->unk18 = 4;
    KeyframeAnim_SetAnim(&w->pos[4], 5);
    KeyframeAnim_SetAnim(&w->pos[5], 7);
    KeyframeAnim_SetAnim(&w->pos[6], 9);
    KeyframeAnim_SetAnim(&w->pos[7], 0xb);
    if (w->unk20) {
        PlayerSelectMenu_SetSprite60CAnim3(w);
        PlayerSelectMenu_SetSprite654Anim4(w);
    } else {
        PlayerSelectMenu_SetSprite60CAnim4(w);
        PlayerSelectMenu_SetSprite654Anim3(w);
    }
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
    w->unk334 = 1;
    t->unk8 = PlayerSelectMenu_State_ScrollIn;
}

#pragma thumb reset

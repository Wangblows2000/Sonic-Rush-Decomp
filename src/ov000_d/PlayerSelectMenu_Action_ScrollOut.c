#include <ov000_d/scene.h>

void PlayerSelectMenu_State_ScrollOut(void);

#pragma thumb on

void PlayerSelectMenu_Action_ScrollOut(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;

    if (t == NULL) {
        t = gTaskCurrent;
    }
    w = t->work;
    w->unk0 = 0;
    KeyframeAnim_SetAnim(&w->pos[4], 6);
    KeyframeAnim_SetAnim(&w->pos[5], 8);
    KeyframeAnim_SetAnim(&w->pos[6], 0xa);
    KeyframeAnim_SetAnim(&w->pos[7], 0xc);
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
    PlayerSelectMenu_SetSlotSpriteNop(w, 0);
    PlayerSelectMenu_SetSlotSpriteNop(w, 1);
    PlayerSelectMenu_SetSlotSpriteNop(w, 2);
    PlayerSelectMenu_SetSprite5C4Anim2(w);
    w->unk334 = 0;
    if (w->unkC == 0 || (u32)(w->unkC - 3) <= 1) {
        SysSound_PlaySE(5);
    }
    t->unk8 = PlayerSelectMenu_State_ScrollOut;
}

#pragma thumb reset

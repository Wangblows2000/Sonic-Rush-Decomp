#include <ov000_d/scene.h>

void PlayerSelectMenu_Action_ScrollIn(s32 a);

#pragma thumb on

void PlayerSelectMenu_State_ScrollOut(void) {
    UnkStruct_Scene *w = gTaskCurrent->work;
    UnkStruct_ScenePos *p;
    s32 ang;
    s32 x;
    s32 step;
    s32 base;
    s32 i;
    s32 t;

    ang = PlayerSelectMenu_EaseValue(0, 0x4000, 0x10, w->unk0, 0x2000);
    if (w->unk20) {
        if (ang > 0x4000) {
            ang = 0x4000;
        }
    } else {
        ang = -ang;
        if (ang < -0x4000) {
            ang = -0x4000;
        }
    }
    PlayerSelectMenu_SetModelSlotSelection(&w->mdl, w->unk14);
    PlayerSelectMenu_SetModelSelection(&w->mdl, 0);
    PlayerSelectMenu_SetCameraOrbit(&w->mdl, ang);
    PlayerSelectMenu_AnimateModels(&w->mdl);
    PlayerSelectMenu_DrawModels(&w->mdl);
    KeyframeAnim_Update(&w->pos[4]);
    KeyframeAnim_Update(&w->pos[5]);
    p = &w->pos[0];
    PlayerSelectMenu_DrawSprite60C(w, p->x, p->y);
    p = &w->pos[1];
    PlayerSelectMenu_DrawSprite654(w, p->x, p->y);
    p = &w->pos[2];
    PlayerSelectMenu_DrawSprite69C(w, p->x, p->y);
    p = &w->pos[3];
    PlayerSelectMenu_DrawSprite6E4(w, p->x, p->y);
    p = &w->pos[4];
    PlayerSelectMenu_DrawSprite9B4(w, p->x, p->y);
    KeyframeAnim_Update(&w->pos[7]);
    x = w->pos[7].x;
    if (!w->unk20) {
        x = -x;
    }
    PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, x);
    if (w->unk10 == 2) {
        step = 0x8000;
    } else {
        step = 0x5555;
    }
    base = PlayerSelectMenu_EaseValue(0, step, 0x20, w->unk0, 0x2000);
    if (w->unk20) {
        base = -base;
    }
    for (i = 0; i < w->unk10; i++) {
        PlayerSelectMenu_DrawOrbitSprite(w, (w->unk14 + i) % w->unk10, base + step * i);
    }
    if (w->unk14 == 2 || w->unk18 == 2) {
        t = (s32)(w->unk0 << 12) / 10;
        if (t < 0x1000) {
            PlayerSelectMenu_DrawBottomSpriteScaled(w, 0, 0x1000 - t);
        }
    } else {
        PlayerSelectMenu_DrawBottomSpriteScaled(w, 0, 0x1000);
    }
    if (w->unk0 < 9) {
        if (w->unk0 < 5) {
            gDisplaySub[2].x++;
            PlayerSelectMenu_DrawSubTitleSprite(w, TRUE);
        } else {
            PlayerSelectMenu_DrawSubTitleSprite(w, FALSE);
        }
    } else {
        PlayerSelectMenu_SetSprite5C4Anim0(w);
        PlayerSelectMenu_DrawSubSpriteAtX(w, 0);
    }
    if ((u32)(w->unkC - 1) <= 1 && w->unk14 == w->unk24) {
        PlayerSelectMenu_DrawSpriteA44(w, 0, 0);
    }
    w->unk0++;
    if (w->unk0 >= 0x10) {
        PlayerSelectMenu_Action_ScrollIn(0);
    }
}

#pragma thumb reset

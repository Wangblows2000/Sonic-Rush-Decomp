#include <game/Menus/scene.h>

void PlayerSelectMenu_Action_Select(s32 a);

#pragma thumb on

void PlayerSelectMenu_State_ScrollIn(void) {
    UnkStruct_Scene *w = gTaskCurrent->work;
    UnkStruct_ScenePos *p;
    s32 ang;
    s32 x;
    s32 t;
    s32 step;
    s32 base;
    s32 i;

    ang = PlayerSelectMenu_EaseValue(0, 0x4000, 0x10, w->unk0, 0x2000);
    if (w->unk20) {
        ang = -(0x4000 - ang);
        if (ang > 0) {
            ang = 0;
        }
    } else {
        ang = 0x4000 - ang;
        if (ang < 0) {
            ang = 0;
        }
    }
    PlayerSelectMenu_SetModelSlotSelection(&w->mdl, w->unk14);
    PlayerSelectMenu_SetModelSelection(&w->mdl, 0);
    PlayerSelectMenu_SetCameraOrbit(&w->mdl, ang);
    PlayerSelectMenu_AnimateModels(&w->mdl);
    PlayerSelectMenu_DrawModels(&w->mdl);
    p = &w->pos[0];
    PlayerSelectMenu_DrawSprite60C(w, p->x, p->y);
    p = &w->pos[1];
    PlayerSelectMenu_DrawSprite654(w, p->x, p->y);
    p = &w->pos[2];
    PlayerSelectMenu_DrawSprite69C(w, p->x, p->y);
    p = &w->pos[3];
    PlayerSelectMenu_DrawSprite6E4(w, p->x, p->y);
    if (w->unk0 >= 6) {
        KeyframeAnim_Update(&w->pos[4]);
        KeyframeAnim_Update(&w->pos[5]);
        p = &w->pos[4];
        PlayerSelectMenu_DrawSprite9B4(w, p->x, p->y);
        KeyframeAnim_Update(&w->pos[6]);
        PlayerSelectMenu_DrawSelectionCursor(w, w->unk14, w->pos[6].unk4);
        KeyframeAnim_Update(&w->pos[7]);
        x = w->pos[7].x;
        if (!w->unk20) {
            x = -x;
        }
        PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, x);
    }
    if (w->unk14 == 2 || w->unk1C == 2) {
        t = (s32)((w->unk0 - 6) << 12) / 10;
    } else {
        t = 0x1000;
    }
    PlayerSelectMenu_DrawBottomSpriteScaled(w, 0, t);
    if (w->unk10 == 2) {
        step = 0x8000;
    } else {
        step = 0x5555;
    }
    base = PlayerSelectMenu_EaseValue(0, step, 0x20, w->unk0 + 0x10, 0x2000);
    if (w->unk20) {
        base = -base;
    }
    for (i = 0; i < w->unk10; i++) {
        PlayerSelectMenu_DrawOrbitSprite(w, (w->unk1C + i) % w->unk10, base + step * i);
    }
    PlayerSelectMenu_DrawSubSpriteAtX(w, 0);
    if ((u32)(w->unkC - 1) <= 1 && w->unk14 == w->unk24) {
        PlayerSelectMenu_DrawSpriteA44(w, 0, 0);
    }
    w->unk0++;
    if (w->unk0 >= 0x10) {
        PlayerSelectMenu_SetCameraOrbit(&w->mdl, 0);
        PlayerSelectMenu_DrawSelectionCursor(w, w->unk14, 0x1000);
        PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, 0);
        PlayerSelectMenu_Action_Select(0);
    }
}

#pragma thumb reset

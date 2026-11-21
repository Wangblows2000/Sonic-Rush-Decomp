#include <ov000_d/scene.h>

typedef struct {
    u16 cont;
    u8 pad2[2];
    u16 trg;
} UnkStruct_023593bc_Pad;

extern UnkStruct_023593bc_Pad gPad;

void PlayerSelectMenu_Action_Select(s32 a);

#pragma thumb on

void PlayerSelectMenu_State_Intro(void) {
    UnkStruct_Scene *w = gTaskCurrent->work;
    UnkStruct_ScenePos *p;
    s32 ang;
    s32 x;
    BOOL done;

    if (w->unkC != 1 && w->unkC != 2 && (gPad.trg & 1)) {
        w->unk340 = 1;
    }
    if (w->unk340) {
        w->unk0 = 0x10;
        KeyframeAnim_SetToLastFrame(&w->pos[0]);
        KeyframeAnim_SetToLastFrame(&w->pos[1]);
        KeyframeAnim_SetToLastFrame(&w->pos[2]);
        KeyframeAnim_SetToLastFrame(&w->pos[3]);
        KeyframeAnim_SetToLastFrame(&w->pos[4]);
        KeyframeAnim_SetToLastFrame(&w->pos[5]);
    }
    ang = (w->unk0 << 14) >> 4;
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
    done = FALSE;
    if (w->unk0 >= 6) {
        KeyframeAnim_Update(&w->pos[6]);
        PlayerSelectMenu_DrawSelectionCursor(w, w->unk14, w->pos[6].unk4);
        KeyframeAnim_Update(&w->pos[7]);
        x = w->pos[7].x;
        if (!w->unk20) {
            x = -x;
        }
        PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, x);
        KeyframeAnim_Update(&w->pos[0]);
        KeyframeAnim_Update(&w->pos[1]);
        KeyframeAnim_Update(&w->pos[2]);
        KeyframeAnim_Update(&w->pos[3]);
        KeyframeAnim_Update(&w->pos[4]);
        KeyframeAnim_Update(&w->pos[5]);
        done = TRUE;
        p = &w->pos[0];
        PlayerSelectMenu_DrawSprite60C(w, p->x, p->y);
        if (!KeyframeAnim_IsFinished(p)) {
            done = FALSE;
        }
        p = &w->pos[1];
        PlayerSelectMenu_DrawSprite654(w, p->x, p->y);
        if (!KeyframeAnim_IsFinished(p)) {
            done = FALSE;
        }
        p = &w->pos[2];
        PlayerSelectMenu_DrawSprite69C(w, p->x, p->y);
        if (!KeyframeAnim_IsFinished(p)) {
            done = FALSE;
        }
        p = &w->pos[3];
        PlayerSelectMenu_DrawSprite6E4(w, p->x, p->y);
        if (!KeyframeAnim_IsFinished(p)) {
            done = FALSE;
        }
        p = &w->pos[4];
        PlayerSelectMenu_DrawSprite9B4(w, p->x, p->y);
        if (!KeyframeAnim_IsFinished(p)) {
            done = FALSE;
        }
        p = &w->pos[5];
        PlayerSelectMenu_DrawSpriteA8C(w, p->x, p->y);
        if (!KeyframeAnim_IsFinished(p)) {
            done = FALSE;
        }
    }
    PlayerSelectMenu_DrawBottomSpriteScaled(w, 0, 0x1000);
    w->unk330 += 0x20;
    if (w->unk330 > 0) {
        w->unk330 = 0;
    }
    PlayerSelectMenu_DrawSubSpriteAtX(w, w->unk330);
    w->unk0++;
    if (done && w->unk0 >= 0x10) {
        PlayerSelectMenu_SetCameraOrbit(&w->mdl, 0);
        PlayerSelectMenu_DrawSelectionCursor(w, w->unk14, 0x1000);
        PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, 0);
        w->unk340 = 0;
        PlayerSelectMenu_Action_Select(0);
    }
}

#pragma thumb reset

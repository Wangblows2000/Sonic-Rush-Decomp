#include <game/Menus/scene.h>

void Task_DestroyByTag(s32 a);
void PlayerSelectMenu_ExitBack(s32 mode, s32 sub);

#pragma thumb on

void PlayerSelectMenu_State_FadeOutBack(void) {
    UnkStruct_Scene *w = gTaskCurrent->work;
    UnkStruct_ScenePos *p;
    u32 step;
    s32 i;

    if (w->unk0 >= 0x28) {
        s32 mode = w->unkC;
        s32 sel = w->unk14;
        Task_DestroyByTag(0x1000);
        PlayerSelectMenu_ExitBack(mode, sel);
        return;
    }
    PlayerSelectMenu_AnimateModels(&w->mdl);
    PlayerSelectMenu_DrawModels(&w->mdl);
    PlayerSelectMenu_DrawSelectionCursor(w, w->unk14, 0x1000);
    PlayerSelectMenu_SetBackgroundSelection(w, w->unk14, 0);
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
    if (w->unkC != 2) {
        p = &w->pos[5];
        PlayerSelectMenu_DrawSpriteA8C(w, p->x, p->y);
    }
    if (w->unk10 == 2) {
        step = 0x8000;
    } else {
        step = 0x5555;
    }
    for (i = 1; i < w->unk10; i++) {
        PlayerSelectMenu_DrawOrbitSprite(w, (w->unk14 + i) % w->unk10, step * i);
    }
    PlayerSelectMenu_DrawSprite804Scaled(w, 0x80, 0x98, 0x1000);
    PlayerSelectMenu_DrawBottomSpriteScaled(w, 0, 0x1000);
    gDisplaySub[2].x++;
    PlayerSelectMenu_DrawSubTitleSprite2(w, TRUE);
    w->unk0++;
}

#pragma thumb reset

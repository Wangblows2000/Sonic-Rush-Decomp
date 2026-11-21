#include <game/Menus/scene.h>

BOOL MsgBox_IsClosed(void);
BOOL MsgBox_GetResult(void);
void Wireless_EnableErrorCallback(s32 a);
void PlayerSelectMenu_SetLocalQuit(UnkStruct_Scene *w);
void PlayerSelectMenu_Action_FadeOutBack(s32 a);
void PlayerSelectMenu_Action_Select(s32 a);

#pragma thumb on

void PlayerSelectMenu_State_QuitPrompt(void) {
    UnkStruct_Scene *w = gTaskCurrent->work;
    UnkStruct_ScenePos *p;
    u32 step;
    s32 i;

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
    p = &w->pos[5];
    PlayerSelectMenu_DrawSpriteA8C(w, p->x, p->y);
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
    if (MsgBox_IsClosed()) {
        if (!MsgBox_GetResult()) {
            Wireless_EnableErrorCallback(0);
            PlayerSelectMenu_SetLocalQuit(w);
            PlayerSelectMenu_Action_FadeOutBack(0);
        } else {
            w->unk338 = 1;
            PlayerSelectMenu_Action_Select(0);
        }
    }
}

#pragma thumb reset

#include <game/Menus/scene.h>

typedef struct {
    u8 pad0[8];
    s16 unk8;
} UnkStruct_02358a18_Bright;

typedef struct {
    u16 cont;
    u8 pad2[2];
    u16 trg;
} UnkStruct_02358a18_Pad;

extern UnkStruct_02358a18_Bright data_02087d44;
extern UnkStruct_02358a18_Bright data_0208926c;
extern UnkStruct_02358a18_Pad gPad;

BOOL PlayerSelectMenu_IsDifferentChoiceFromPartner(UnkStruct_Scene *w);
BOOL PlayerSelectMenu_IsSameChoiceAsPartner(UnkStruct_Scene *w);
BOOL PlayerSelectMenu_HasPartnerCancelled(UnkStruct_Scene *w);
void Task_DestroyByTag(s32 a);
void PlayerSelectMenu_ExitToGame(s32 mode, s32 sub);
void Task_DestroyRange(s32 a, s32 b);
void PlayerSelectMenu_StartBrightnessFade(s32 x, s32 y, s32 a, s32 b);
void SysSound_StopBGM(s32 a);
void SysSound_PlayMenuVoice(s32 a);
void PlayerSelectMenu_Action_Select(s32 a);
void PlayerSelectMenu_Action_PartnerLeft(s32 a);

#pragma thumb on

void PlayerSelectMenu_State_Confirmed(void) {
    UnkStruct_Scene *w = gTaskCurrent->work;
    UnkStruct_ScenePos *p;
    u32 step;
    s32 i;
    s32 limit;
    BOOL back = FALSE;
    BOOL next = FALSE;

    if ((u32)(w->unkC - 1) <= 1) {
        if (w->unk4 >= 0xe2) {
            if (PlayerSelectMenu_IsDifferentChoiceFromPartner(w)) {
                s32 mode = w->unkC;
                s32 sel = w->unk14;
                Task_DestroyByTag(0x1000);
                PlayerSelectMenu_ExitToGame(mode, sel);
                return;
            }
            Task_DestroyRange(0x3000, 0x3000);
            data_02087d44.unk8 = 0;
            data_0208926c.unk8 = data_02087d44.unk8;
            back = TRUE;
        }
        if (w->unkC == 2 && PlayerSelectMenu_IsSameChoiceAsPartner(w)) {
            back = TRUE;
        }
        if (PlayerSelectMenu_HasPartnerCancelled(w)) {
            next = TRUE;
        }
    } else {
        if (w->unk14 == 2) {
            limit = 0x78;
        } else {
            limit = 0xe2;
        }
        if (limit - w->unk0 > 0x10 && w->unk0 >= 0x20 && (gPad.trg & 1)) {
            w->unk0 = limit - 0x10;
        }
        if (limit - w->unk0 == 0x10) {
            PlayerSelectMenu_StartBrightnessFade(0, 0x10, 0x10, 0);
            if (w->unkC == 0) {
                SysSound_StopBGM(0x10);
            }
        }
        if (w->unk0 >= limit) {
            s32 mode = w->unkC;
            s32 sel = w->unk14;
            Task_DestroyByTag(0x1000);
            PlayerSelectMenu_ExitToGame(mode, sel);
            return;
        }
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
    p = &w->pos[5];
    PlayerSelectMenu_DrawSpriteA8C(w, p->x, p->y);
    PlayerSelectMenu_DrawSprite9FC(w);
    if (w->unk10 == 2) {
        step = 0x8000;
    } else {
        step = 0x5555;
    }
    for (i = 1; i < w->unk10; i++) {
        PlayerSelectMenu_DrawOrbitSprite(w, (w->unk14 + i) % w->unk10, step * i);
    }
    PlayerSelectMenu_DrawSprite804(w, 0x80, 0x98);
    PlayerSelectMenu_DrawBottomSpriteScaled(w, 0, 0x1000);
    gDisplaySub[2].x++;
    PlayerSelectMenu_DrawSubTitleSprite2(w, TRUE);
    if (w->unk0 == 8 && (u32)(w->unkC - 1) <= 1) {
        switch (w->unk14) {
        case 0:
            SysSound_PlayMenuVoice(6);
            break;
        case 1:
            SysSound_PlayMenuVoice(7);
            break;
        }
    }
    w->unk0++;
    if ((u32)(w->unkC - 1) <= 1) {
        if (PlayerSelectMenu_IsDifferentChoiceFromPartner(w)) {
            if (w->unk8 > 0x10) {
                if (w->unk4 == 0) {
                    PlayerSelectMenu_StartBrightnessFade(0, 0x10, 0x10, 0xd2);
                }
                w->unk4++;
            }
            w->unk8++;
        } else {
            w->unk8 = 0;
            w->unk4 = 0;
        }
    }
    if ((u32)(w->unkC - 1) <= 1 && w->unk4 == 0 && w->unk8 == 0 && (gPad.trg & 2)) {
        back = TRUE;
    }
    if (back) {
        PlayerSelectMenu_Action_Select(0);
    } else if (next) {
        PlayerSelectMenu_Action_PartnerLeft(0);
    }
}

#pragma thumb reset

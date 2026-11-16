#include <ov000_d/scene.h>

void PlayerSelectMenu_Action_ScrollOut(s32 a);
BOOL PlayerSelectMenu_IsConfirmInput(void);
void PlayerSelectMenu_Action_Confirm(s32 a);
BOOL PlayerSelectMenu_IsLeftInput(void);
BOOL PlayerSelectMenu_IsRightInput(void);
s32 PlayerSelectMenu_GetPartnerChoice(UnkStruct_Scene *w);
BOOL PlayerSelectMenu_IsBackTouched(void);
BOOL PlayerSelectMenu_IsBackPressed(void);
BOOL PlayerSelectMenu_HasPartnerCancelled(UnkStruct_Scene *w);
void SysSound_PlayCancelSE(void);
void PlayerSelectMenu_Action_OpenQuitPrompt(s32 a);
void PlayerSelectMenu_Action_FadeOutBack(s32 a);
void PlayerSelectMenu_Action_PartnerLeft(s32 a);
void func_ov000_02357cf8(UnkStruct_Scene *w);
void func_ov000_02357c5c(UnkStruct_Scene *w);

#pragma thumb on

void PlayerSelectMenu_State_Select(void) {
    UnkStruct_Scene *w = gTaskCurrent->work;
    UnkStruct_ScenePos *p;
    u32 step;
    s32 i;
    s32 scale;
    s32 n;

    PlayerSelectMenu_SetModelSelection(&w->mdl, 0);
    PlayerSelectMenu_AnimateModels(&w->mdl);
    PlayerSelectMenu_DrawModels(&w->mdl);
    func_ov000_02357ec8(w, w->unk14, 0x1000);
    func_ov000_023582e8(w, w->unk14, 0);
    p = &w->pos[0];
    func_ov000_023579fc(w, p->x, p->y);
    p = &w->pos[1];
    func_ov000_023579c8(w, p->x, p->y);
    p = &w->pos[2];
    func_ov000_0235793c(w, p->x, p->y);
    p = &w->pos[3];
    func_ov000_02357908(w, p->x, p->y);
    p = &w->pos[4];
    func_ov000_02357994(w, p->x, p->y);
    func_ov000_02357970(w);
    if (w->unk0 != 0 || w->unk338 == 1) {
        p = &w->pos[5];
        func_ov000_023578d0(w, p->x, p->y);
        w->unk338 = 0;
    }
    if (w->unk10 == 2) {
        step = 0x8000;
    } else {
        step = 0x5555;
    }
    for (i = 1; i < w->unk10; i++) {
        func_ov000_02358384(w, (w->unk14 + i) % w->unk10, step * i);
    }
    if (w->unk334 == 0 || w->unk0 >= 4) {
        scale = 0x1000;
    } else {
        scale = (s32)(w->unk0 * 0x600) / 4 + 0xa00;
    }
    func_ov000_02357878(w, 0x80, 0x98, scale);
    func_ov000_02358224(w, 0, 0x1000);
    if (w->unk334 == 1 && w->unk0 < 5) {
        func_ov000_023580bc(w, FALSE);
    } else {
        gDisplaySub[2].x++;
        func_ov000_023580bc(w, TRUE);
    }
    if ((u32)(w->unkC - 1) <= 1 && w->unk14 == w->unk24) {
        func_ov000_02357810(w, 0, 0);
    }
    w->unk0++;
    if ((u32)(w->unkC - 1) <= 1 && w->unk14 == w->unk24) {
        n = w->unk14;
        if (w->unk20) {
            if (++n >= w->unk10) {
                n = 0;
            }
        } else {
            if (--n < 0) {
                n = w->unk10 - 1;
            }
        }
        w->unk18 = n;
        PlayerSelectMenu_Action_ScrollOut(0);
        return;
    }
    if (PlayerSelectMenu_IsConfirmInput()) {
        PlayerSelectMenu_Action_Confirm(0);
        return;
    }
    if (PlayerSelectMenu_IsLeftInput()) {
        n = w->unk14 - 1;
        if (n < 0) {
            n = w->unk10 - 1;
        }
        w->unk20 = 0;
        w->unk18 = n;
        PlayerSelectMenu_Action_ScrollOut(0);
        return;
    }
    if (PlayerSelectMenu_IsRightInput()) {
        n = w->unk14 + 1;
        if (n >= w->unk10) {
            n = 0;
        }
        w->unk20 = 1;
        w->unk18 = n;
        PlayerSelectMenu_Action_ScrollOut(0);
        return;
    }
    if ((u32)(w->unkC - 1) <= 1) {
        n = PlayerSelectMenu_GetPartnerChoice(w);
        if (w->unk24 != n) {
            w->unk24 = n;
        }
    }
    if (w->unkC != 2) {
        if (PlayerSelectMenu_IsBackTouched()) {
            w->unk33C = 1;
            SysSound_PlayCancelSE();
            if (w->unkC == 1) {
                PlayerSelectMenu_Action_OpenQuitPrompt(0);
            } else {
                PlayerSelectMenu_Action_FadeOutBack(0);
            }
            return;
        }
        if (PlayerSelectMenu_IsBackPressed()) {
            w->unk33C = 0;
            SysSound_PlayCancelSE();
            if (w->unkC == 1) {
                PlayerSelectMenu_Action_OpenQuitPrompt(0);
            } else {
                PlayerSelectMenu_Action_FadeOutBack(0);
            }
            return;
        }
    } else {
        if (PlayerSelectMenu_HasPartnerCancelled(w)) {
            SysSound_PlayCancelSE();
            PlayerSelectMenu_Action_PartnerLeft(0);
            return;
        }
    }
    if (w->unk0 == 1) {
        func_ov000_02357cf8(w);
        func_ov000_02357c5c(w);
    }
}

#pragma thumb reset

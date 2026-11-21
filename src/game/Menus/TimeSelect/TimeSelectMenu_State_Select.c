#include <game/Menus/menuo.h>

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
} UnkStruct_02361bf0_Sync;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;

UnkStruct_02361bf0_Sync *WirelessSession_GetSendBuffer(void);
void TimeSelectMenu_SetSprite78Anim2Hidden(UnkStruct_MenuO *w);
void TimeSelectMenu_SetSpriteC0Anim2Hidden(UnkStruct_MenuO *w);
BOOL TimeSelectMenu_IsConfirmInput(UnkStruct_MenuO *w);
void SysSound_PlaySE(s32 a);
void MsgBox_RequestClose(void);
void TimeSelectMenu_SetSprite30Anim1(UnkStruct_MenuO *w);
void TimeSelectMenu_SetSprite1SelectAnim(UnkStruct_MenuO *w, s32 a, s32 b);
void TimeSelectMenu_State_ChooserConfirmed(void);
BOOL TimeSelectMenu_IsBackTouched(UnkStruct_MenuO *w);
BOOL TimeSelectMenu_IsBackPressed(UnkStruct_MenuO *w);
void TimeSelectMenu_SetSprite2Anim1(UnkStruct_MenuO *w);
void TimeSelectMenu_StartBrightnessFade(s32 x, s32 y, s32 a, s32 b);
void TimeSelectMenu_State_ExitToPlayerSelect(void);
BOOL TimeSelectMenu_IsLeftInput(UnkStruct_MenuO *w);
BOOL TimeSelectMenu_IsRightInput(UnkStruct_MenuO *w);
void TimeSelectMenu_SetSprite78Anim3Hidden(UnkStruct_MenuO *w);
void TimeSelectMenu_SetSpriteC0Anim3Hidden(UnkStruct_MenuO *w);
void TimeSelectMenu_SetSprite1Anim(UnkStruct_MenuO *w, s32 a, s32 b);
void TimeSelectMenu_SetSprite0Anim(UnkStruct_MenuO *w, s32 a);
void TimeSelectMenu_State_WaitScroll(void);

#pragma thumb on

void TimeSelectMenu_State_Select(void) {
    UnkStruct_MenuO *w = gTaskCurrent->work;
    s32 old;
    s32 n;
    u16 mask;
    s32 q;

    TimeSelectMenu_DrawLayoutA(w);
    TimeSelectMenu_DrawLayoutB(w, 0x1000, 1);
    w->unk0++;
    if (w->unk0 == 1 && w->unk8 > 1) {
        TimeSelectMenu_SetSprite78Anim2Hidden(w);
        TimeSelectMenu_SetSpriteC0Anim2Hidden(w);
    }
    if (TimeSelectMenu_IsConfirmInput(w) || w->unk18) {
        UnkStruct_02361bf0_Sync *s;
        w->unk0 = 0;
        SysSound_PlaySE(7);
        s = WirelessSession_GetSendBuffer();
        s->unk0 = w->unkC;
        s->unk1 = 1;
        MsgBox_RequestClose();
        w->unk14 = 0;
        TimeSelectMenu_SetSprite30Anim1(w);
        n = w->unkC;
        TimeSelectMenu_SetSprite1SelectAnim(w, n / 2, n % 2);
        gGameState.unk1C = w->unkC / 2;
        gGameState.unk1D = w->unkC % 2;
        gTaskCurrent->unk8 = TimeSelectMenu_State_ChooserConfirmed;
        return;
    }
    if (TimeSelectMenu_IsBackTouched(w) || TimeSelectMenu_IsBackPressed(w)) {
        w->unk0 = 0;
        SysSound_PlaySE(8);
        WirelessSession_GetSendBuffer()->unk2 = 1;
        if (TimeSelectMenu_IsBackTouched(w)) {
            TimeSelectMenu_SetSprite2Anim1(w);
        }
        TimeSelectMenu_StartBrightnessFade(0, 0x10, 0x10, 0);
        gTaskCurrent->unk8 = TimeSelectMenu_State_ExitToPlayerSelect;
        return;
    }
    if (TimeSelectMenu_IsLeftInput(w)) {
        w->unk0 = 0;
        old = w->unkC;
        n = old;
        mask = w->unk24;
        do {
            if (--n < 0) {
                n = 0xd;
            }
        } while (!(mask & (1 << n)));
        if (old != n) {
            w->unk10 = old;
            w->unkC = n;
            WirelessSession_GetSendBuffer()->unk0 = w->unkC;
            KeyframeAnim_SetAnim(&w->pos[0], 6);
            TimeSelectMenu_SetSprite78Anim3Hidden(w);
            q = n / 2;
            TimeSelectMenu_SetSprite1Anim(w, q, n % 2);
            if (w->unk10 / 2 != q) {
                TimeSelectMenu_SetSprite0Anim(w, q);
            }
            gTaskCurrent->unk8 = TimeSelectMenu_State_WaitScroll;
        }
        return;
    }
    if (TimeSelectMenu_IsRightInput(w)) {
        w->unk0 = 0;
        old = w->unkC;
        n = old;
        mask = w->unk24;
        do {
            if (++n > 0xd) {
                n = 0;
            }
        } while (!(mask & (1 << n)));
        if (old != n) {
            w->unk10 = old;
            w->unkC = n;
            WirelessSession_GetSendBuffer()->unk0 = w->unkC;
            KeyframeAnim_SetAnim(&w->pos[0], 6);
            TimeSelectMenu_SetSpriteC0Anim3Hidden(w);
            q = n / 2;
            TimeSelectMenu_SetSprite1Anim(w, q, n % 2);
            if (w->unk10 / 2 != q) {
                TimeSelectMenu_SetSprite0Anim(w, q);
            }
            gTaskCurrent->unk8 = TimeSelectMenu_State_WaitScroll;
        }
    }
}

#pragma thumb reset

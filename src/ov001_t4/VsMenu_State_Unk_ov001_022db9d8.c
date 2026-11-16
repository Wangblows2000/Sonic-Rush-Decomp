#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[8];
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    s32 unk1c;
} UnkWork_022db9d8;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022db9d8 *work;
} UnkTask_022db9d8;

extern UnkTask_022db9d8 *gTaskCurrent;

void VsMenu_Unk_ov001_022d9eb4(UnkWork_022db9d8 *w);
void VsMenu_DrawParentList(UnkWork_022db9d8 *w);
s32 VsMenu_GetTouchedListRow(void);
void VsMenu_RefreshParentList(UnkWork_022db9d8 *w);
s32 WirelessLobbyChild_GetParentCount(void);
void VsMenu_Unk_ov001_022d9e74(UnkWork_022db9d8 *w);
BOOL VsMenu_IsUpRepeated(void);
BOOL VsMenu_IsDownRepeated(void);
BOOL VsMenu_IsBackButtonTouched(void);
BOOL VsMenu_IsBPressed(void);
BOOL VsMenu_IsAPressed(void);
void VsMenu_Unk_ov001_022da304(UnkWork_022db9d8 *w);
void SysSound_PlayCancelSE(void);
void SysSound_PlayDecideSE(void);
void SysSound_PlaySE(s32 a);
void VsMenu_PrepareMsgBox(UnkWork_022db9d8 *w);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void VsMenu_State_ChildConfirmJoin(void);
void VsMenu_State_ChildConfirmCancel(void);
void VsMenu_State_Unk_ov001_022db8d0(void);

#pragma thumb on
void VsMenu_State_Unk_ov001_022db9d8(void) {
    UnkWork_022db9d8 *w = gTaskCurrent->work;
    s32 sel;
    BOOL cancel;
    BOOL empty;

    if (w->unk4 == 3) {
        VsMenu_Unk_ov001_022d9eb4(w);
    }
    VsMenu_DrawParentList(w);
    sel = VsMenu_GetTouchedListRow();
    cancel = FALSE;
    empty = FALSE;
    if (w->unk1c == 0) {
        VsMenu_RefreshParentList(w);
        w->unk14 = WirelessLobbyChild_GetParentCount();
        if (w->unk14 > 8) {
            w->unk14 = 8;
        }
        if (w->unk10 < 0) {
            if (w->unk14 > 0) {
                w->unk10 = 0;
                w->unk4 = 0;
                VsMenu_Unk_ov001_022d9e74(w);
            }
        } else if (w->unk14 == 0) {
            w->unk10 = -1;
            w->unk4 = 0;
            empty = TRUE;
        } else if (w->unk10 >= w->unk14) {
            w->unk10 = w->unk14 - 1;
            w->unk4 = 0;
            VsMenu_Unk_ov001_022d9e74(w);
        } else if (VsMenu_IsUpRepeated()) {
            if (w->unk10 > 0) {
                w->unk10--;
                w->unk4 = 0;
                VsMenu_Unk_ov001_022d9e74(w);
                SysSound_PlaySE(4);
            }
        } else if (VsMenu_IsDownRepeated()) {
            if (w->unk10 < w->unk14 - 1) {
                w->unk10++;
                w->unk4 = 0;
                VsMenu_Unk_ov001_022d9e74(w);
                SysSound_PlaySE(4);
            }
        } else if (VsMenu_IsBackButtonTouched()) {
            VsMenu_Unk_ov001_022da304(w);
            cancel = TRUE;
            SysSound_PlayCancelSE();
        } else if (VsMenu_IsBPressed()) {
            cancel = TRUE;
            SysSound_PlayCancelSE();
        } else if (sel >= 0 && sel < w->unk14) {
            w->unk10 = sel;
            w->unk4 = 0;
            VsMenu_Unk_ov001_022d9e74(w);
            w->unk1c = 1;
            SysSound_PlayDecideSE();
        } else if (VsMenu_IsAPressed()) {
            w->unk4 = 0;
            w->unk1c = 1;
            SysSound_PlayDecideSE();
        }
    }
    w->unk0++;
    w->unk4++;
    if (w->unk1c != 0 && w->unk4 >= 30) {
        w->unk0 = 0;
        w->unk4 = 0;
        w->unk1c = 0;
        VsMenu_Unk_ov001_022d9eb4(w);
        VsMenu_PrepareMsgBox(w);
        MsgBox_Create(5, 3, 1, 1, 2, 0, 0xe, 0x200010);
        gTaskCurrent->func = VsMenu_State_ChildConfirmJoin;
        return;
    }
    if (cancel) {
        w->unk0 = 0;
        w->unk4 = 0;
        w->unk1c = 0;
        VsMenu_Unk_ov001_022d9eb4(w);
        VsMenu_PrepareMsgBox(w);
        MsgBox_Create(9, 3, 1, 1, 2, 0, 0xe, 0x200010);
        gTaskCurrent->func = VsMenu_State_ChildConfirmCancel;
        return;
    }
    if (empty) {
        w->unk0 = 0;
        w->unk4 = 0;
        w->unk1c = 0;
        VsMenu_Unk_ov001_022d9eb4(w);
        gTaskCurrent->func = VsMenu_State_Unk_ov001_022db8d0;
    }
}
#pragma thumb reset

#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x1c - 8];
    s32 unk1c;
    u8 pad20[0x48 - 0x20];
    s32 unk48;
    u8 unk4c[4];
} UnkWork_022dc580;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dc580 *work;
} UnkTask_022dc580;

extern UnkTask_022dc580 *gTaskCurrent;

void VsMenu_DrawDownloadPlayScreen(UnkWork_022dc580 *w);
BOOL WirelessLobbyDownloadPlay_IsFinished(void);
BOOL WirelessLobbyDownloadPlay_GetChildCount(void);
void VsMenu_PlayErrorSE(void);
BOOL VsMenu_IsBackButtonTouched(void);
void VsMenu_Unk_ov001_022da304(UnkWork_022dc580 *w);
void SysSound_PlayCancelSE(void);
BOOL VsMenu_IsBPressed(void);
void VsMenu_PrepareMsgBox(UnkWork_022dc580 *w);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void VsMenu_State_ParentConfirmRestart(void);
void VsMenu_State_ParentConfirmStart(void);
void VsMenu_State_Unk_ov001_022dc498(void);
void VsMenu_DrawParentScreen(UnkWork_022dc580 *w);
s32 WirelessLobbyParent_GetChildName(u8 *buf, s32 n);
BOOL WirelessLobbyParent_IsWaitingForChild(void);
s32 WirelessLobbyDownloadPlay_GetChildName(s32 a, u8 *buf, s32 n);
void VsMenu_BuildGlyphString(void *p, u8 *buf, u16 len);
void VsMenu_Unk_ov001_022d9cd4(UnkWork_022dc580 *w, s32 a, s32 b);
BOOL VsMenu_IsStartButtonTouched(void);
void VsMenu_Unk_ov001_022d9fe4(UnkWork_022dc580 *w);
void SysSound_PlayDecideSE(void);
BOOL VsMenu_IsStartPressed(void);
void VsMenu_Unk_ov001_022da064(UnkWork_022dc580 *w, s32 a);
void VsMenu_State_Unk_ov001_022dcc10(void);
void VsMenu_Unk_ov001_022da33c(UnkWork_022dc580 *w);
void VsMenu_Unk_ov001_022da244(UnkWork_022dc580 *w, s32 a);
void VsMenu_Unk_ov001_022da1cc(UnkWork_022dc580 *w, s32 a);
void VsMenu_ClearNameSlot2(UnkWork_022dc580 *w, s32 a);
void VsMenu_State_DownloadPlayRestartDelay(void);

#pragma thumb on
void VsMenu_State_Unk_ov001_022dc580(void) {
    UnkWork_022dc580 *w;
    BOOL a;
    BOOL c;
    u8 buf[16];

    w = gTaskCurrent->work;
    VsMenu_DrawParentScreen(w);
    VsMenu_Unk_ov001_022d9cd4(w, 0, 0);
    VsMenu_BuildGlyphString(w->unk4c, buf, WirelessLobbyParent_GetChildName(buf, 7));
    a = FALSE;
    c = FALSE;
    if (w->unk1c == 0) {
        if (VsMenu_IsBackButtonTouched()) {
            VsMenu_Unk_ov001_022da304(w);
            a = TRUE;
            SysSound_PlayCancelSE();
        } else if (VsMenu_IsBPressed()) {
            a = TRUE;
            SysSound_PlayCancelSE();
        }
        if (VsMenu_IsStartButtonTouched()) {
            VsMenu_Unk_ov001_022d9fe4(w);
            w->unk1c = 1;
            w->unk4 = 0;
            SysSound_PlayDecideSE();
        } else if (VsMenu_IsStartPressed()) {
            w->unk1c = 1;
            w->unk4 = 0x10;
            SysSound_PlayDecideSE();
        } else if (WirelessLobbyParent_IsWaitingForChild()) {
            c = TRUE;
        }
    }
    w->timer++;
    w->unk4++;
    if (a) {
        w->timer = 0;
        w->unk4 = 0;
        VsMenu_PrepareMsgBox(w);
        MsgBox_Create(1, 3, 1, 1, 2, 0, 0xe, 0x200010);
        gTaskCurrent->func = VsMenu_State_ParentConfirmRestart;
        return;
    }
    if (w->unk1c != 0 && w->unk4 > 0x10) {
        w->timer = 0;
        w->unk4 = 0;
        w->unk1c = 0;
        VsMenu_PrepareMsgBox(w);
        MsgBox_Create(6, 3, 1, 1, 2, 0, 0xe, 0x200010);
        gTaskCurrent->func = VsMenu_State_ParentConfirmStart;
        return;
    }
    if (c) {
        w->timer = 0;
        w->unk4 = 0;
        gTaskCurrent->func = VsMenu_State_Unk_ov001_022dc498;
    }
}
#pragma thumb reset

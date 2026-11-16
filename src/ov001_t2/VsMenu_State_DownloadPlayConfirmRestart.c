#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x48 - 8];
    s32 unk48;
} UnkWork_022dcd24;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_022dcd24 *work;
} UnkTask_022dcd24;

extern UnkTask_022dcd24 *gTaskCurrent;

void VsMenu_DrawDownloadPlayScreen(UnkWork_022dcd24 *w);
BOOL WirelessLobbyDownloadPlay_IsFinished(void);
void MsgBox_RequestClose(void);
void WirelessLobbyDownloadPlay_RequestRestart(void);
BOOL MsgBox_IsClosed(void);
void VsMenu_RestoreAfterMsgBox(UnkWork_022dcd24 *w);
BOOL MsgBox_GetResult(void);
void VsMenu_Unk_ov001_022da33c(UnkWork_022dcd24 *w);
void VsMenu_Unk_ov001_022da244(UnkWork_022dcd24 *w, s32 a);
void VsMenu_Unk_ov001_022da1cc(UnkWork_022dcd24 *w, s32 a);
void VsMenu_ClearNameSlot2(UnkWork_022dcd24 *w, s32 i);
void VsMenu_State_Unk_ov001_022dd1ec(void);
void VsMenu_State_Unk_ov001_022dcdbc(void);

#pragma thumb on
void VsMenu_State_DownloadPlayConfirmRestart(void) {
    UnkWork_022dcd24 *w = gTaskCurrent->work;

    VsMenu_DrawDownloadPlayScreen(w);
    w->timer++;
    if (WirelessLobbyDownloadPlay_IsFinished()) {
        MsgBox_RequestClose();
    }
    if (MsgBox_IsClosed()) {
        w->timer = 0;
        w->unk4 = 0;
        VsMenu_RestoreAfterMsgBox(w);
        if (!WirelessLobbyDownloadPlay_IsFinished() && !MsgBox_GetResult()) {
            WirelessLobbyDownloadPlay_RequestRestart();
            VsMenu_Unk_ov001_022da33c(w);
            VsMenu_Unk_ov001_022da244(w, 1);
            VsMenu_Unk_ov001_022da1cc(w, 1);
            w->unk48 = 1;
            VsMenu_ClearNameSlot2(w, 1);
            gTaskCurrent->fn = VsMenu_State_Unk_ov001_022dd1ec;
            return;
        }
        VsMenu_Unk_ov001_022da33c(w);
        gTaskCurrent->fn = VsMenu_State_Unk_ov001_022dcdbc;
    }
}
#pragma thumb reset

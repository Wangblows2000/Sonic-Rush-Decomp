#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 timer2;
    u8 pad8[0x1c - 8];
    s32 unk1c;
    u8 pad20[0x48 - 0x20];
    s32 unk48;
} UnkWork_022dcec8;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dcec8 *work;
} UnkTask_022dcec8;

extern UnkTask_022dcec8 *gTaskCurrent;

void VsMenu_DrawDownloadPlayScreen(UnkWork_022dcec8 *w);
void VsMenu_Unk_ov001_022da024(UnkWork_022dcec8 *w);
void VsMenu_Unk_ov001_022d9cd4(UnkWork_022dcec8 *w, s32 a, s32 b);
BOOL WirelessLobbyDownloadPlay_GetChildCount(void);
void MsgBox_RequestClose(void);
BOOL MsgBox_IsClosed(void);
void VsMenu_RestoreAfterMsgBox(UnkWork_022dcec8 *w);
void VsMenu_Unk_ov001_022da33c(UnkWork_022dcec8 *w);
void VsMenu_Unk_ov001_022da244(UnkWork_022dcec8 *w, s32 a);
void VsMenu_Unk_ov001_022da1cc(UnkWork_022dcec8 *w, s32 a);
void VsMenu_ClearNameSlot2(UnkWork_022dcec8 *w, s32 a);
BOOL MsgBox_GetResult(void);
void WirelessLobbyDownloadPlay_RequestStart(void);
void VsMenu_Unk_ov001_022da0dc(UnkWork_022dcec8 *w, s32 a);
void VsMenu_Unk_ov001_022daaa8(UnkWork_022dcec8 *w);
void VsMenu_State_Unk_ov001_022dd1ec(void);
void VsMenu_State_Unk_ov001_022dcdbc(void);

#pragma thumb on
void VsMenu_State_Unk_ov001_022dcec8(void) {
    UnkWork_022dcec8 *w;

    w = gTaskCurrent->work;
    VsMenu_DrawDownloadPlayScreen(w);
    if (w->timer2 >= 0x10) {
        VsMenu_Unk_ov001_022da024(w);
    }
    VsMenu_Unk_ov001_022d9cd4(w, 0, 0);
    if (!WirelessLobbyDownloadPlay_GetChildCount()) {
        MsgBox_RequestClose();
        w->unk1c = 1;
    }
    w->timer++;
    w->timer2++;
    if (MsgBox_IsClosed()) {
        w->timer = 0;
        w->timer2 = 0;
        VsMenu_RestoreAfterMsgBox(w);
        if (w->unk1c == 1) {
            w->unk1c = 0;
            VsMenu_Unk_ov001_022da33c(w);
            VsMenu_Unk_ov001_022da244(w, 1);
            VsMenu_Unk_ov001_022da1cc(w, 1);
            w->unk48 = 1;
            VsMenu_ClearNameSlot2(w, 1);
            gTaskCurrent->func = VsMenu_State_Unk_ov001_022dd1ec;
            return;
        }
        if (!MsgBox_GetResult()) {
            WirelessLobbyDownloadPlay_RequestStart();
            VsMenu_Unk_ov001_022da0dc(w, 1);
            w->unk48 = 2;
            gTaskCurrent->func = VsMenu_State_Unk_ov001_022dcdbc;
            return;
        }
        VsMenu_Unk_ov001_022daaa8(w);
    }
}
#pragma thumb reset

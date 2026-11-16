#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x48 - 8];
    s32 unk48;
} UnkWork_022dcdbc;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dcdbc *work;
} UnkTask_022dcdbc;

extern UnkTask_022dcdbc *gTaskCurrent;

void VsMenu_Unk_ov001_022da70c(UnkWork_022dcdbc *w);
BOOL WirelessLobbyDownloadPlay_IsFinished(void);
BOOL WirelessLobbyDownloadPlay_GetChildCount(void);
void VsMenu_PlayErrorSE(void);
BOOL VsMenu_Unk_ov001_022da52c(void);
void VsMenu_Unk_ov001_022da304(UnkWork_022dcdbc *w);
void SysSound_PlayCancelSE(void);
BOOL VsMenu_IsBPressed(void);
void VsMenu_Unk_ov001_022da450(UnkWork_022dcdbc *w);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void VsMenu_State_Unk_ov001_022dcd24(void);
void VsMenu_Unk_ov001_022da064(UnkWork_022dcdbc *w, s32 a);
void VsMenu_State_Unk_ov001_022dcc10(void);
void VsMenu_Unk_ov001_022da33c(UnkWork_022dcdbc *w);
void VsMenu_Unk_ov001_022da244(UnkWork_022dcdbc *w, s32 a);
void VsMenu_Unk_ov001_022da1cc(UnkWork_022dcdbc *w, s32 a);
void VsMenu_Unk_ov001_022d983c(UnkWork_022dcdbc *w, s32 a);
void VsMenu_State_Unk_ov001_022dccc8(void);

#pragma thumb on
void VsMenu_State_Unk_ov001_022dcdbc(void) {
    UnkWork_022dcdbc *w;
    BOOL a;
    BOOL b;
    BOOL c;

    w = gTaskCurrent->work;
    VsMenu_Unk_ov001_022da70c(w);
    a = FALSE;
    b = FALSE;
    c = FALSE;
    if (WirelessLobbyDownloadPlay_IsFinished()) {
        b = TRUE;
    } else if (!WirelessLobbyDownloadPlay_GetChildCount()) {
        c = TRUE;
        VsMenu_PlayErrorSE();
    } else if (VsMenu_Unk_ov001_022da52c()) {
        VsMenu_Unk_ov001_022da304(w);
        a = TRUE;
        SysSound_PlayCancelSE();
    } else if (VsMenu_IsBPressed()) {
        a = TRUE;
        SysSound_PlayCancelSE();
    }
    w->timer++;
    if (a) {
        w->timer = 0;
        w->unk4 = 0;
        VsMenu_Unk_ov001_022da450(w);
        MsgBox_Create(8, 3, 1, 1, 2, 0, 0xe, 0x200010);
        gTaskCurrent->func = VsMenu_State_Unk_ov001_022dcd24;
        return;
    }
    if (b) {
        w->timer = 0;
        w->unk4 = 0;
        VsMenu_Unk_ov001_022da064(w, 1);
        w->unk48 = 3;
        gTaskCurrent->func = VsMenu_State_Unk_ov001_022dcc10;
        return;
    }
    if (c) {
        w->timer = 0;
        w->unk4 = 0;
        VsMenu_Unk_ov001_022da33c(w);
        VsMenu_Unk_ov001_022da244(w, 1);
        VsMenu_Unk_ov001_022da1cc(w, 1);
        VsMenu_Unk_ov001_022d983c(w, 1);
        gTaskCurrent->func = VsMenu_State_Unk_ov001_022dccc8;
    }
}
#pragma thumb reset

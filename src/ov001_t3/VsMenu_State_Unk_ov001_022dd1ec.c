#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x1c - 8];
    s32 unk1c;
    u8 pad20[0x48 - 0x20];
    s32 unk48;
    u8 unk4c[4];
} UnkWork_022dd1ec;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dd1ec *work;
} UnkTask_022dd1ec;

extern UnkTask_022dd1ec *gTaskCurrent;

void VsMenu_DrawDownloadPlayScreen(UnkWork_022dd1ec *w);
BOOL VsMenu_IsBackButtonTouched(void);
void VsMenu_Unk_ov001_022da304(UnkWork_022dd1ec *w);
void SysSound_PlayCancelSE(void);
BOOL VsMenu_IsBPressed(void);
s32 WirelessLobbyDownloadPlay_GetChildCount(void);
void VsMenu_PrepareMsgBox(UnkWork_022dd1ec *w);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void VsMenu_State_DownloadPlayConfirmStop(void);
s32 WirelessLobbyDownloadPlay_GetChildName(s32 a, u8 *buf, s32 n);
void VsMenu_BuildGlyphString(void *p, u8 *buf, u16 len);
void VsMenu_Unk_ov001_022da024(UnkWork_022dd1ec *w);
void VsMenu_Unk_ov001_022da288(UnkWork_022dd1ec *w, s32 a);
void VsMenu_Unk_ov001_022da154(UnkWork_022dd1ec *w, s32 a);
void VsMenu_PlayDecideSE(void);
void VsMenu_State_Unk_ov001_022dd050(void);

#pragma thumb on
void VsMenu_State_Unk_ov001_022dd1ec(void) {
    UnkWork_022dd1ec *w;
    BOOL a;
    BOOL b;
    u8 buf[16];

    w = gTaskCurrent->work;
    VsMenu_DrawDownloadPlayScreen(w);
    a = FALSE;
    b = FALSE;
    if (VsMenu_IsBackButtonTouched()) {
        VsMenu_Unk_ov001_022da304(w);
        a = TRUE;
        SysSound_PlayCancelSE();
    } else if (VsMenu_IsBPressed()) {
        a = TRUE;
        SysSound_PlayCancelSE();
    } else if (WirelessLobbyDownloadPlay_GetChildCount() > 0) {
        b = TRUE;
    }
    w->timer++;
    if (a) {
        w->timer = 0;
        w->unk4 = 0;
        VsMenu_PrepareMsgBox(w);
        MsgBox_Create(1, 3, 1, 1, 2, 0, 0xe, 0x200010);
        gTaskCurrent->func = VsMenu_State_DownloadPlayConfirmStop;
        return;
    }
    if (b) {
        w->timer = 0;
        w->unk4 = 0;
        VsMenu_BuildGlyphString(w->unk4c, buf, WirelessLobbyDownloadPlay_GetChildName(0, buf, 7));
        VsMenu_Unk_ov001_022da024(w);
        VsMenu_Unk_ov001_022da288(w, 1);
        VsMenu_Unk_ov001_022da154(w, 1);
        w->unk48 = 0;
        VsMenu_PlayDecideSE();
        gTaskCurrent->func = VsMenu_State_Unk_ov001_022dd050;
    }
}
#pragma thumb reset

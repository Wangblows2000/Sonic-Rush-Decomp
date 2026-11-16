#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x1c - 8];
    s32 unk1c;
    u8 pad20[0x48 - 0x20];
    s32 unk48;
    u8 unk4c[4];
} UnkWork_022dc734;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dc734 *work;
} UnkTask_022dc734;

extern UnkTask_022dc734 *gTaskCurrent;

void VsMenu_Unk_ov001_022da694(UnkWork_022dc734 *w);
BOOL VsMenu_Unk_ov001_022da52c(void);
void VsMenu_Unk_ov001_022da304(UnkWork_022dc734 *w);
void SysSound_PlayCancelSE(void);
BOOL VsMenu_IsBPressed(void);
BOOL WirelessLobbyParent_HasChild(void);
void VsMenu_Unk_ov001_022da450(UnkWork_022dc734 *w);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void VsMenu_State_Unk_ov001_022dc6d8(void);
s32 WirelessLobbyParent_GetChildName(u8 *buf, s32 n);
void VsMenu_BuildGlyphString(void *p, u8 *buf, u16 len);
void VsMenu_Unk_ov001_022da024(UnkWork_022dc734 *w);
void VsMenu_Unk_ov001_022da288(UnkWork_022dc734 *w, s32 a);
void VsMenu_Unk_ov001_022da154(UnkWork_022dc734 *w, s32 a);
void VsMenu_PlayDecideSE(void);
void VsMenu_State_Unk_ov001_022dc580(void);

#pragma thumb on
void VsMenu_State_Unk_ov001_022dc734(void) {
    UnkWork_022dc734 *w;
    BOOL a;
    BOOL b;
    u8 buf[16];

    w = gTaskCurrent->work;
    VsMenu_Unk_ov001_022da694(w);
    a = FALSE;
    b = FALSE;
    if (VsMenu_Unk_ov001_022da52c()) {
        VsMenu_Unk_ov001_022da304(w);
        a = TRUE;
        SysSound_PlayCancelSE();
    } else if (VsMenu_IsBPressed()) {
        a = TRUE;
        SysSound_PlayCancelSE();
    } else if (WirelessLobbyParent_HasChild()) {
        b = TRUE;
    }
    w->timer++;
    if (a) {
        w->timer = 0;
        w->unk4 = 0;
        VsMenu_Unk_ov001_022da450(w);
        MsgBox_Create(1, 3, 1, 1, 2, 0, 0xe, 0x200010);
        gTaskCurrent->func = VsMenu_State_Unk_ov001_022dc6d8;
        return;
    }
    if (b) {
        w->timer = 0;
        w->unk4 = 0;
        VsMenu_BuildGlyphString(w->unk4c, buf, WirelessLobbyParent_GetChildName(buf, 7));
        VsMenu_Unk_ov001_022da024(w);
        VsMenu_Unk_ov001_022da288(w, 1);
        VsMenu_Unk_ov001_022da154(w, 1);
        w->unk48 = 0;
        w->unk1c = 0;
        VsMenu_PlayDecideSE();
        gTaskCurrent->func = VsMenu_State_Unk_ov001_022dc580;
    }
}
#pragma thumb reset

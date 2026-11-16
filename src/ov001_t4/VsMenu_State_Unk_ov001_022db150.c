#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
} UnkAnim_022db150;

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[0xb44 - 8];
    UnkAnim_022db150 anims[6];
} UnkWork_022db150;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022db150 *work;
} UnkTask_022db150;

extern UnkTask_022db150 *gTaskCurrent;

void VsMenu_Unk_ov001_022da59c(UnkWork_022db150 *w);
void VsMenu_Unk_ov001_022d9cb0(UnkWork_022db150 *w, s32 a, s32 b);
BOOL VsMenu_IsBackButtonTouched(void);
BOOL VsMenu_IsBPressed(void);
BOOL WirelessLobbyChild_IsFinished(void);
BOOL WirelessLobbyChild_IsBeforeConnect(void);
void VsMenu_PlayErrorSE(void);
void VsMenu_Unk_ov001_022da304(UnkWork_022db150 *w);
void SysSound_PlayCancelSE(void);
void VsMenu_PrepareMsgBox(UnkWork_022db150 *w);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void VsMenu_Unk_ov001_022da374(UnkWork_022db150 *w);
void VsMenu_Unk_ov001_022da33c(UnkWork_022db150 *w);
void VsMenu_Unk_ov001_022da2cc(UnkWork_022db150 *w, s32 i);
void VsMenu_Unk_ov001_022da288(UnkWork_022db150 *w, s32 i);
void KeyframeAnim_SetAnim(UnkAnim_022db150 *a, s32 id);
void TitleMenu_Unk_ov001_022d6dc0(void);
void VsMenu_CreateBrightnessFade(s32 a, s32 b, s32 c, s32 d);
void SysSound_PlaySEDelayed(s32 a, s32 b, s32 c, s32 d);
void VsMenu_State_Unk_ov001_022db014(void);
void VsMenu_State_Unk_ov001_022dac0c(void);
void VsMenu_State_Unk_ov001_022dafdc(void);

#pragma thumb on
void VsMenu_State_Unk_ov001_022db150(void) {
    UnkWork_022db150 *w = gTaskCurrent->work;
    BOOL cancel;
    BOOL ok;
    BOOL other;

    VsMenu_Unk_ov001_022da59c(w);
    VsMenu_Unk_ov001_022d9cb0(w, 0, 0);
    cancel = FALSE;
    ok = FALSE;
    other = FALSE;
    if (VsMenu_IsBackButtonTouched()) {
        VsMenu_Unk_ov001_022da304(w);
        cancel = TRUE;
        SysSound_PlayCancelSE();
    } else if (VsMenu_IsBPressed()) {
        cancel = TRUE;
        SysSound_PlayCancelSE();
    } else if (WirelessLobbyChild_IsFinished()) {
        ok = TRUE;
    } else if (WirelessLobbyChild_IsBeforeConnect()) {
        other = TRUE;
        VsMenu_PlayErrorSE();
    }
    w->unk0++;
    if (cancel) {
        w->unk0 = 0;
        w->unk4 = 0;
        VsMenu_PrepareMsgBox(w);
        MsgBox_Create(3, 3, 1, 1, 2, 0, 0xe, 0x200010);
        gTaskCurrent->func = VsMenu_State_Unk_ov001_022db014;
        return;
    }
    if (ok) {
        w->unk0 = 0;
        w->unk4 = 0;
        VsMenu_Unk_ov001_022da374(w);
        VsMenu_Unk_ov001_022da33c(w);
        VsMenu_Unk_ov001_022da2cc(w, 0);
        VsMenu_Unk_ov001_022da2cc(w, 1);
        VsMenu_Unk_ov001_022da288(w, 0);
        VsMenu_Unk_ov001_022da288(w, 1);
        KeyframeAnim_SetAnim(&w->anims[0], 0xe);
        KeyframeAnim_SetAnim(&w->anims[1], 0xf);
        KeyframeAnim_SetAnim(&w->anims[2], 0x10);
        KeyframeAnim_SetAnim(&w->anims[3], 0x11);
        KeyframeAnim_SetAnim(&w->anims[4], 0x12);
        KeyframeAnim_SetAnim(&w->anims[5], 0x13);
        TitleMenu_Unk_ov001_022d6dc0();
        VsMenu_CreateBrightnessFade(0, 0x10, 0x10, 0xc);
        SysSound_PlaySEDelayed(2, 0xc, 0x3000, 0);
        gTaskCurrent->func = VsMenu_State_Unk_ov001_022dac0c;
        return;
    }
    if (other) {
        w->unk0 = 0;
        w->unk4 = 0;
        VsMenu_PrepareMsgBox(w);
        MsgBox_Create(2, 3, 2, 1, 2, 0, 0xe, 0x200010);
        gTaskCurrent->func = VsMenu_State_Unk_ov001_022dafdc;
    }
}
#pragma thumb reset

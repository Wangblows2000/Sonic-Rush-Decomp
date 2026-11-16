#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
} UnkAnim_022dc324;

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x48 - 8];
    s32 unk48;
    u8 pad4c[0xb44 - 0x4c];
    UnkAnim_022dc324 anims[6];
} UnkWork_022dc324;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dc324 *work;
} UnkTask_022dc324;

extern UnkTask_022dc324 *gTaskCurrent;

void VsMenu_DrawParentScreen(UnkWork_022dc324 *w);
BOOL WirelessLobbyParent_IsFinished(void);
BOOL WirelessLobbyParent_IsBeforeStart(void);
void KeyframeAnim_SetAnim(UnkAnim_022dc324 *a, s32 id);
void TitleMenu_Unk_ov001_022d6dc0(void);
void VsMenu_CreateBrightnessFade(s32 a, s32 b, s32 c, s32 d);
void SysSound_PlaySEDelayed(s32 a, s32 b, s32 c, s32 d);
void VsMenu_State_Unk_ov001_022dc0b4(void);
void VsMenu_Unk_ov001_022da33c(UnkWork_022dc324 *w);
void VsMenu_Unk_ov001_022da244(UnkWork_022dc324 *w, s32 a);
void VsMenu_Unk_ov001_022da1cc(UnkWork_022dc324 *w, s32 a);
void VsMenu_ClearNameSlot2(UnkWork_022dc324 *w, s32 a);
void VsMenu_PlayErrorSE(void);
void VsMenu_State_Unk_ov001_022dc734(void);

#pragma thumb on
void VsMenu_State_ParentWaitStart(void) {
    UnkWork_022dc324 *w;

    w = gTaskCurrent->work;
    VsMenu_DrawParentScreen(w);
    w->timer++;
    if (WirelessLobbyParent_IsFinished()) {
        w->timer = 0;
        w->unk4 = 0;
        w->unk48 = 0;
        KeyframeAnim_SetAnim(&w->anims[0], 0xe);
        KeyframeAnim_SetAnim(&w->anims[1], 0xf);
        KeyframeAnim_SetAnim(&w->anims[2], 0x10);
        KeyframeAnim_SetAnim(&w->anims[3], 0x11);
        KeyframeAnim_SetAnim(&w->anims[4], 0x12);
        KeyframeAnim_SetAnim(&w->anims[5], 0x13);
        TitleMenu_Unk_ov001_022d6dc0();
        VsMenu_CreateBrightnessFade(0, 0x10, 0x10, 0xc);
        SysSound_PlaySEDelayed(2, 0xc, 0x3000, 0);
        gTaskCurrent->func = VsMenu_State_Unk_ov001_022dc0b4;
        return;
    }
    if (WirelessLobbyParent_IsBeforeStart()) {
        VsMenu_Unk_ov001_022da33c(w);
        VsMenu_Unk_ov001_022da244(w, 1);
        VsMenu_Unk_ov001_022da1cc(w, 1);
        w->unk48 = 1;
        VsMenu_ClearNameSlot2(w, 1);
        VsMenu_PlayErrorSE();
        gTaskCurrent->func = VsMenu_State_Unk_ov001_022dc734;
    }
}
#pragma thumb reset

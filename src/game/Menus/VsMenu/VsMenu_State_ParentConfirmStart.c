#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x48 - 8];
    s32 unk48;
} UnkWork_022dc410;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_022dc410 *work;
} UnkTask_022dc410;

extern UnkTask_022dc410 *gTaskCurrent;

void VsMenu_DrawParentScreen(UnkWork_022dc410 *w);
BOOL WirelessLobbyParent_IsWaitingForChild(void);
void MsgBox_RequestClose(void);
BOOL MsgBox_IsClosed(void);
void VsMenu_RestoreAfterMsgBox(UnkWork_022dc410 *w);
BOOL MsgBox_GetResult(void);
void WirelessLobbyParent_RequestStart(void);
void VsMenu_Unk_ov001_022da33c(UnkWork_022dc410 *w);
void VsMenu_Unk_ov001_022da244(UnkWork_022dc410 *w, s32 a);
void VsMenu_Unk_ov001_022da1cc(UnkWork_022dc410 *w, s32 a);
void VsMenu_ClearNameSlot2(UnkWork_022dc410 *w, s32 i);
void VsMenu_State_ParentWaitStart(void);
void VsMenu_State_Unk_ov001_022dc734(void);

#pragma thumb on
void VsMenu_State_ParentConfirmStart(void) {
    UnkWork_022dc410 *w = gTaskCurrent->work;

    VsMenu_DrawParentScreen(w);
    if (WirelessLobbyParent_IsWaitingForChild()) {
        MsgBox_RequestClose();
    }
    w->timer++;
    if (MsgBox_IsClosed()) {
        w->timer = 0;
        w->unk4 = 0;
        VsMenu_RestoreAfterMsgBox(w);
        if (!MsgBox_GetResult()) {
            WirelessLobbyParent_RequestStart();
            gTaskCurrent->fn = VsMenu_State_ParentWaitStart;
            return;
        }
        VsMenu_Unk_ov001_022da33c(w);
        VsMenu_Unk_ov001_022da244(w, 1);
        VsMenu_Unk_ov001_022da1cc(w, 1);
        w->unk48 = 1;
        VsMenu_ClearNameSlot2(w, 1);
        gTaskCurrent->fn = VsMenu_State_Unk_ov001_022dc734;
    }
}
#pragma thumb reset

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 VsMenu_Unk_ov001_022da614();
s32 MsgBox_IsClosed();
s32 VsMenu_Unk_ov001_022da41c();
s32 MsgBox_GetResult();
s32 WirelessLobbyChild_RequestCancel();
s32 VsMenu_Unk_ov001_022da91c();
s32 VsMenu_Unk_ov001_022da33c();
void VsMenu_State_Unk_ov001_022db9d8();

void VsMenu_State_Unk_ov001_022db97c(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    VsMenu_Unk_ov001_022da614(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    s32 t1 = (*(u32 *)t0 + 0x1);
    *(u32 *)t0 = t1;
    s32 t2 = MsgBox_IsClosed(t1);
    if (t2 != 0) {
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)t0 + 0x4) = 0;
        VsMenu_Unk_ov001_022da41c(t0);
        s32 t3 = MsgBox_GetResult();
        if (t3 == 0) {
            WirelessLobbyChild_RequestCancel();
            VsMenu_Unk_ov001_022da91c(t0);
            return;
        }
        VsMenu_Unk_ov001_022da33c(t0);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)VsMenu_State_Unk_ov001_022db9d8;
    }
}

#pragma thumb reset

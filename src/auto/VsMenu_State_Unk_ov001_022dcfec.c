/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 VsMenu_DrawDownloadPlayScreen();
s32 VsMenu_Unk_ov001_022d9cd4();
s32 MsgBox_IsClosed();
s32 VsMenu_RestoreAfterMsgBox();
s32 MsgBox_GetResult();
s32 VsMenu_Unk_ov001_022daaa8();
s32 VsMenu_Unk_ov001_022da33c();
void VsMenu_State_Unk_ov001_022dd050();

void VsMenu_State_Unk_ov001_022dcfec(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    VsMenu_DrawDownloadPlayScreen(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    VsMenu_Unk_ov001_022d9cd4(t0, 0, 0);
    s32 t1 = (*(u32 *)t0 + 0x1);
    *(u32 *)t0 = t1;
    s32 t2 = MsgBox_IsClosed(t1);
    if (t2 != 0) {
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)t0 + 0x4) = 0;
        VsMenu_RestoreAfterMsgBox(t0);
        s32 t3 = MsgBox_GetResult();
        if (t3 == 0) {
            VsMenu_Unk_ov001_022daaa8(t0);
            return;
        }
        VsMenu_Unk_ov001_022da33c(t0);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)VsMenu_State_Unk_ov001_022dd050;
    }
}

#pragma thumb reset

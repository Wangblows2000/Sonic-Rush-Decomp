/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gPad[];
s32 MsgBox_RequestClose();
void SaveMsg_State_WaitFormatDoneClose();

void SaveMsg_State_WaitStart(u8 *p0) {
    if ((*(u16 *)((u8 *)gPad + 0x4) & 8) == 0) {
        return;
    }
    MsgBox_RequestClose((*(u16 *)((u8 *)gPad + 0x4) & 8), gPad);
    *(u32 *)p0 = (u32)SaveMsg_State_WaitFormatDoneClose;
}

/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 MsgBox_IsClosed();
void SaveMsg_State_ShowFormatDone();

void SaveMsg_State_WaitFormatClose(u8 *p0) {
    s32 t0 = MsgBox_IsClosed();
    if (t0 != 0) {
        *(u32 *)p0 = (u32)SaveMsg_State_ShowFormatDone;
    }
}

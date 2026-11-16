/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 MsgBox_Create();
void SaveMsg_State_FormatDoneDelay();

void SaveMsg_State_ShowFormatDone(u8 *p0) {
    MsgBox_Create(0x18, 3, 5, 0, 1, 1, 0, 0x100010);
    *(u16 *)((u8 *)p0 + 0x10) = 0xb4;
    *(u32 *)p0 = (u32)SaveMsg_State_FormatDoneDelay;
}

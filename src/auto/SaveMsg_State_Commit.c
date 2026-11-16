/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 SaveData_Commit();
s32 MsgBox_RequestClose();
void SaveMsg_State_WaitCommitClose();

void SaveMsg_State_Commit(u8 *p0) {
    s32 t0 = SaveData_Commit();
    s32 v1 = t0;
    if (t0 != 0) {
        s32 t2 = (*(u32 *)((u8 *)p0 + 0x8) | 1);
        *(u32 *)((u8 *)p0 + 0x8) = t2;
        v1 = t2;
    }
    MsgBox_RequestClose(v1);
    *(u32 *)p0 = (u32)SaveMsg_State_WaitCommitClose;
}

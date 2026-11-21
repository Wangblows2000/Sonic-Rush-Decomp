/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 SaveData_Format();
s32 MsgBox_RequestClose();
void SaveMsg_State_WaitFormatClose();
extern u8 gTaskCurrent[];
s32 Task_Destroy();
s32 SaveMsg_StopSystems();
s32 SaveMsg_Request();

void SaveMsg_State_Format(u8 *p0) {
    s32 t0 = SaveData_Format();
    if (t0 == 0) {
        MsgBox_RequestClose();
        *(u32 *)p0 = (u32)SaveMsg_State_WaitFormatClose;
        return;
    }
    Task_Destroy(*(u32 *)gTaskCurrent);
    SaveMsg_StopSystems();
    SaveMsg_Request(2, 0);
}

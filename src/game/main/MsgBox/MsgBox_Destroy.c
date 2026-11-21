/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gMsgBoxTask[];
s32 Task_Destroy();

void MsgBox_Destroy(void) {
    s32 t0 = *(u32 *)gMsgBoxTask;
    if (t0 != 0) {
        Task_Destroy(t0);
        *(u32 *)gMsgBoxTask = 0;
    }
}

#pragma thumb reset

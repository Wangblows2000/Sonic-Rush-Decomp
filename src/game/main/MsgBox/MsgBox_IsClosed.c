#include <nitro/types.h>

extern s32 gMsgBoxTask;

#pragma thumb on
BOOL MsgBox_IsClosed(void) {
    if (gMsgBoxTask == 0) return TRUE;
    return FALSE;
}
#pragma thumb reset

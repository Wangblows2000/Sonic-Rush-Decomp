#include <nitro/types.h>

#pragma thumb on

extern u32 gMsgBoxResult;
u32 MsgBox_GetResult(void) { return gMsgBoxResult; }

#pragma thumb reset

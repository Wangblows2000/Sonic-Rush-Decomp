#include <nitro/types.h>

#pragma thumb on

void MsgBox_FreeCachedFile(s32 i);
void MsgBox_ReleaseCache(void) { s32 i; for (i = 0; i < 13; i++) MsgBox_FreeCachedFile(i); }

#pragma thumb reset

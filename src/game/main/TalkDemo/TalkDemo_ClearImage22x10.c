#include <nitro/types.h>

#pragma thumb on

void TalkDemo_ClearMapRect(s32 a, s32 b, s32 c, s32 d, void *e);
void TalkDemo_ClearImage22x10(void) { TalkDemo_ClearMapRect(0, 0, 0x16, 10, (void *)0x6003000); }

#pragma thumb reset

#include <nitro/types.h>

#pragma thumb on

void TalkDemo_ClearMapRect(s32 a, s32 b, s32 c, s32 d, void *e);
void TalkDemo_ClearImage10x10(void) { TalkDemo_ClearMapRect(0, 0, 10, 10, (void *)0x6002000); }

#pragma thumb reset

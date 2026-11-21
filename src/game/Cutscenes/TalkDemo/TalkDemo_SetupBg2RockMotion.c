#include <nitro/types.h>

#pragma thumb on

void TalkDemo_SetupBg2ExitMotion(void *a, s32 b);
BOOL TalkDemo_SetupBg2RockMotion(void *a) { TalkDemo_SetupBg2ExitMotion(a, 5); return TRUE; }

#pragma thumb reset

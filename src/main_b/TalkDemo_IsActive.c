#include <nitro/types.h>

#pragma thumb on

extern void *gTalkDemoTask;
BOOL TalkDemo_IsActive(void) { if (gTalkDemoTask != NULL) return TRUE; return FALSE; }

#pragma thumb reset

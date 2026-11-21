#include <nitro/types.h>

#pragma thumb on

BOOL TalkDemo_IsActive(void);
void DemoScript_StartDelay(void *s);
void DemoScript_State_WaitTalkDemo(void *s) { if (!TalkDemo_IsActive()) DemoScript_StartDelay(s); }

#pragma thumb reset

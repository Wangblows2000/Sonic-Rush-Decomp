#include <nitro/types.h>

#pragma thumb on

BOOL PlayerDemo_IsActive(void);
void DemoScript_StartDelay(void *s);
void DemoScript_State_WaitPlayerDemo(void *s) { if (!PlayerDemo_IsActive()) DemoScript_StartDelay(s); }

#pragma thumb reset

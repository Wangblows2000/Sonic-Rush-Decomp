#include <nitro/types.h>

#pragma thumb on

u32 TalkDemo_GetFlags(s32 i);
void TalkDemo_State_Start(void);
void TalkDemo_State_WaitWindowOpen(void (**p)(void)) { if (TalkDemo_GetFlags(1) & 4) *p = TalkDemo_State_Start; }

#pragma thumb reset

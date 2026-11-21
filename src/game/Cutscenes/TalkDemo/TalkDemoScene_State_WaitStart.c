#include <nitro/types.h>

#pragma thumb on

u32 TalkDemo_GetFlags(s32 i);
void TalkDemoScene_State_StartPage(void);
void TalkDemoScene_State_WaitStart(void (**p)(void)) { if (TalkDemo_GetFlags(3) & 1) *p = TalkDemoScene_State_StartPage; }

#pragma thumb reset

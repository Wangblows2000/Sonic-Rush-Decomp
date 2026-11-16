#include <nitro/types.h>

#pragma thumb on

void TalkDemo_SetFlags(s32 a, u32 b);
void TalkDemo_State_Run(void);
void TalkDemo_State_Start(void (**p)(void)) { TalkDemo_SetFlags(3, 1); *p = TalkDemo_State_Run; }

#pragma thumb reset

#include <nitro/types.h>

#pragma thumb on

void TalkDemo_SetFlags(s32 i, u32 v);
u32 TalkDemo_GetFlags(s32 i);
void TalkDemo_FadeOutBGM(void *s);
void TalkDemo_State_WaitClose(void);
void TalkDemo_State_BeginClose(void (**s)(void)) { TalkDemo_SetFlags(2, 0x10); TalkDemo_SetFlags(1, 0x10); if (!(TalkDemo_GetFlags(0) & 2)) TalkDemo_SetFlags(3, 0x20000000); TalkDemo_FadeOutBGM(s); *s = TalkDemo_State_WaitClose; }

#pragma thumb reset

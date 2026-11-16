#include <nitro/types.h>

#pragma thumb on

void TalkDemo_SetFlags(s32 i, u32 v);
u32 TalkDemo_GetFlags(s32 i);
void func_020607c8(void *s);
void func_02060ba4(void);
void func_02060bd8(void (**s)(void)) { TalkDemo_SetFlags(2, 0x10); TalkDemo_SetFlags(1, 0x10); if (!(TalkDemo_GetFlags(0) & 2)) TalkDemo_SetFlags(3, 0x20000000); func_020607c8(s); *s = func_02060ba4; }

#pragma thumb reset

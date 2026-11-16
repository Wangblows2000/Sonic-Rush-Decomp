#include <nitro/types.h>

#pragma thumb on

u32 TalkDemo_GetFlags(s32 i);
void func_02062ae4(void);
void func_02062b5c(void (**p)(void)) { if (TalkDemo_GetFlags(3) & 1) *p = func_02062ae4; }

#pragma thumb reset

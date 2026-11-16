#include <nitro/types.h>

#pragma thumb on

void TalkDemo_SetFlags(s32 a, u32 b);
void func_02060c1c(void);
void func_02060cc4(void (**p)(void)) { TalkDemo_SetFlags(3, 1); *p = func_02060c1c; }

#pragma thumb reset

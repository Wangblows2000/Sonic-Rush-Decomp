#include <nitro/types.h>

#pragma thumb on

u32 func_02060e70(s32 i);
void func_02060cc4(void);
void func_02060ce0(void (**p)(void)) { if (func_02060e70(1) & 4) *p = func_02060cc4; }

#pragma thumb reset

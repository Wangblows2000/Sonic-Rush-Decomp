#include <nitro/types.h>

#pragma thumb on

u32 func_02060e70(s32 i);
void func_02062ae4(void);
void func_02062b5c(void (**p)(void)) { if (func_02060e70(3) & 1) *p = func_02062ae4; }

#pragma thumb reset

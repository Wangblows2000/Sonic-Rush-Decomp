#include <nitro/types.h>

#pragma thumb on

void func_02060e54(s32 a, u32 b);
void func_02060c1c(void);
void func_02060cc4(void (**p)(void)) { func_02060e54(3, 1); *p = func_02060c1c; }

#pragma thumb reset

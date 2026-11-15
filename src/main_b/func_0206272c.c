#include <nitro/types.h>

#pragma thumb on

void func_02062714(void);
void func_0206272c(void (**p)(void)) { *p = func_02062714; }

#pragma thumb reset

#include <nitro/types.h>

#pragma thumb on

void func_02058168(void);
void func_0205817c(void (**p)(void)) { *p = func_02058168; }

#pragma thumb reset

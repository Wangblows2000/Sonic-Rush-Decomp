#include <nitro/types.h>

#pragma thumb on

void func_02056b74(void);
void func_02056c9c(void);
void func_02056ce0(void (**p)(void)) { func_02056b74(); *p = func_02056c9c; }

#pragma thumb reset

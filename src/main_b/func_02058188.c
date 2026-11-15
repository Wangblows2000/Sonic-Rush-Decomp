#include <nitro/types.h>

#pragma thumb on

void func_0205817c(void (**p)(void));
void func_02058188(void (**p)(void)) { *p = (void (*)(void))func_0205817c; }

#pragma thumb reset

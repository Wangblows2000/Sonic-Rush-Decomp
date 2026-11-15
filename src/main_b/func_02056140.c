#include <nitro/types.h>

#pragma thumb on

void func_02055eb4(void *a);
void func_020560d0(void *a, void *b);
void func_02056140(void *a, void *b) { func_02055eb4(a); func_020560d0(a, b); }

#pragma thumb reset

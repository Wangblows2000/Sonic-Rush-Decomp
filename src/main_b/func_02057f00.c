#include <nitro/types.h>

#pragma thumb on

void func_020572b4(void *a, s32 b);
void func_02057f00(void *a, void *b) { func_020572b4(b, 7); }

#pragma thumb reset

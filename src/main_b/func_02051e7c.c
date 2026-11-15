#include <nitro/types.h>

#pragma thumb on

extern u8 *data_022c465c;
void func_0201c070(void *p, s32 n);
void func_02051e7c(s32 n) { func_0201c070(data_022c465c + 0x14, n); }

#pragma thumb reset

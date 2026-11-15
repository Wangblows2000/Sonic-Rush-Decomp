#include <nitro/types.h>

#pragma thumb on

BOOL func_02057350(void);
void func_02048594(void *s);
void func_02048634(void *s) { if (!func_02057350()) func_02048594(s); }

#pragma thumb reset

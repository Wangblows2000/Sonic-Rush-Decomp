#include <nitro/types.h>

#pragma thumb on

BOOL TalkDemo_IsActive(void);
void func_02048594(void *s);
void func_020485ac(void *s) { if (!TalkDemo_IsActive()) func_02048594(s); }

#pragma thumb reset

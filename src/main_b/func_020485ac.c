// cc: 1.2/sp2p3 (thumb pop {r3}; bx r3 epilogue: matches 1.2/base..sp2p3, not sp3)
#include <nitro/types.h>

#pragma thumb on

BOOL func_02060e8c(void);
void func_02048594(void *s);
void func_020485ac(void *s) { if (!func_02060e8c()) func_02048594(s); }

#pragma thumb reset

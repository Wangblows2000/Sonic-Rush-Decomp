// cc: 1.2/sp2p3 (thumb pop {r3}; bx r3 epilogue: matches 1.2/base..sp2p3, not sp3)
#include <nitro/types.h>

#pragma thumb on

void func_02061b94(void *a, s32 b);
BOOL func_02061b80(void *a) { func_02061b94(a, 5); return TRUE; }

#pragma thumb reset

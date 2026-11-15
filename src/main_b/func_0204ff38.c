// cc: 1.2/sp2p3 (thumb pop {r3}; bx r3 epilogue: matches 1.2/base..sp2p3, not sp3)
#include <nitro/types.h>

#pragma thumb on

void func_0204f540(s32 i);
void func_0204ff38(void) { s32 i; for (i = 0; i < 13; i++) func_0204f540(i); }

#pragma thumb reset

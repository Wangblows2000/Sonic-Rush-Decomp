// cc: 1.2/sp2p3 (thumb pop {r3}; bx r3 epilogue: matches 1.2/base..sp2p3, not sp3)
#include <nitro/types.h>

#pragma thumb on

void func_02056b74(void);
void func_02056c9c(void);
void func_02056ce0(void (**p)(void)) { func_02056b74(); *p = func_02056c9c; }

#pragma thumb reset

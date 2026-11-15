#include <nitro/types.h>

s32 func_02010af8(void);
void func_02010cd4(u32 i, u32 v);
s32 func_02010b80(u32 a, u32 b);
s32 func_02011b10(u32 v) { s32 r = func_02010af8(); if (r != 0) return r; func_02010cd4(1, v); r = func_02010b80(1, 0); if (r == 0) r = 2; return r; }

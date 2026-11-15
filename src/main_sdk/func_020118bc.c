#include <nitro/types.h>

s32 func_02010a50(u32 a, u32 b);
void func_02010cd4(u32 i, u32 v);
s32 func_02010b80(u32 a, u32 b);
s32 func_020118bc(u32 v) { s32 r = func_02010a50(1, 7); if (r != 0) return r; func_02010cd4(9, v); r = func_02010b80(9, 0); if (r == 0) r = 2; return r; }

#include <nitro/types.h>

s32 WMi_CheckStateEx(u32 a, u32 b);
void func_02010cd4(u32 i, u32 v);
s32 func_02010b80(u32 a, u32 b);
s32 WM_End(u32 v) { s32 r = WMi_CheckStateEx(1, 2); if (r != 0) return r; func_02010cd4(2, v); r = func_02010b80(2, 0); if (r == 0) r = 2; return r; }

#include <nitro/types.h>

s32 WMi_CheckStateEx(u32 a, u32 b);
void WMi_SetCallbackTable(u32 i, u32 v);
s32 WMi_SendCommand(u32 a, u32 b);
s32 WM_End(u32 v) { s32 r = WMi_CheckStateEx(1, 2); if (r != 0) return r; WMi_SetCallbackTable(2, v); r = WMi_SendCommand(2, 0); if (r == 0) r = 2; return r; }

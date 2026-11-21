#include <nitro/types.h>

s32 WMi_CheckStateEx(u32 a, u32 b);
void WMi_SetCallbackTable(u32 i, u32 v);
s32 WMi_SendCommand(u32 a, u32 b);
s32 WM_EndParent(u32 v) { s32 r = WMi_CheckStateEx(1, 7); if (r != 0) return r; WMi_SetCallbackTable(9, v); r = WMi_SendCommand(9, 0); if (r == 0) r = 2; return r; }

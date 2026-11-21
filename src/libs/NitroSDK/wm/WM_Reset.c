#include <nitro/types.h>

s32 WMi_CheckIdle(void);
void WMi_SetCallbackTable(u32 i, u32 v);
s32 WMi_SendCommand(u32 a, u32 b);
s32 WM_Reset(u32 v) { s32 r = WMi_CheckIdle(); if (r != 0) return r; WMi_SetCallbackTable(1, v); r = WMi_SendCommand(1, 0); if (r == 0) r = 2; return r; }

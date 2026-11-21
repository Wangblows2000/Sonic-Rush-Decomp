#include <nitro/types.h>

s32 WM_SetMPDataToPortEx(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);
void MBi_CheckWmErrcode(u32 a, s32 b);
s32 MBi_SetMPData(u32 a, u32 c, u32 d, u32 pad, u16 e) { s32 r = WM_SetMPDataToPortEx(a, 0, c, d, e, 1, 3); MBi_CheckWmErrcode(0xf, r); return r; }

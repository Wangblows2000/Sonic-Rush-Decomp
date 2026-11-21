#include <nitro/types.h>

u32 WMi_StartParentEx(u32 a, u32 b);
u32 WM_StartParent(u32 a) { return WMi_StartParentEx(a, 1); }

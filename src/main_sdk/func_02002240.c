#include <nitro/types.h>

void FX_InvAsync(s32 d);
s32 FX_GetDivResult(void);
s32 func_02002240(s32 d) { FX_InvAsync(d); return FX_GetDivResult(); }

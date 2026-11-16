#include <nitro/types.h>

void FX_DivAsync(s32 n, s32 d);
s32 FX_GetDivResult(void);
s32 FX_Div(s32 n, s32 d) { FX_DivAsync(n, d); return FX_GetDivResult(); }

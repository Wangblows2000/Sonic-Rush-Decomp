#include <nitro/types.h>

void func_0203fc88(void);
s32 func_02012d20(void (*cb)(void));
void WH_SetError(s32 v);
BOOL WH_StateInEndParentKeyShare(void) { s32 r = func_02012d20(func_0203fc88); if (r == 2) return TRUE; WH_SetError(r); return FALSE; }

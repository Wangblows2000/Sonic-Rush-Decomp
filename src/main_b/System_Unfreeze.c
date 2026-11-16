#include <nitro/types.h>

extern u32 gSystemFlags;
void func_02038a00(s32 a);
void func_02038534(s32 a);
void System_Unfreeze(void) { func_02038a00(0); func_02038a00(1); func_02038534(0); gSystemFlags &= ~8; }

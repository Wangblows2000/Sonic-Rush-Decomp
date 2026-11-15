#include <nitro/types.h>

extern u32 data_02087cb8;
void func_02038a00(s32 a);
void func_02038534(s32 a);
void func_02038914(void) { func_02038a00(0); func_02038a00(1); func_02038534(0); data_02087cb8 &= ~8; }

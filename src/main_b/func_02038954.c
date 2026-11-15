#include <nitro/types.h>

extern u32 data_02087cb8;
void func_020389b4(BOOL on);
void func_02038534(u32 v);
void func_02038954(BOOL a, BOOL b, BOOL c) { if (a) func_020389b4(0); if (b) func_020389b4(1); if (c) func_02038534(1); data_02087cb8 |= 8; }

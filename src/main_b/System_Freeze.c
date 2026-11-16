#include <nitro/types.h>

extern u32 gSystemFlags;
void func_020389b4(BOOL on);
void func_02038534(u32 v);
void System_Freeze(BOOL a, BOOL b, BOOL c) { if (a) func_020389b4(0); if (b) func_020389b4(1); if (c) func_02038534(1); gSystemFlags |= 8; }

#include <nitro/types.h>

void PMi_WriteRegister(u32 a, u16 b);
void PM_SetAmp(s32 v) { PMi_WriteRegister(2, (u16)v); }

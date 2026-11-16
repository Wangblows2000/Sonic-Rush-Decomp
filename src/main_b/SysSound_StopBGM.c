#include <nitro/types.h>

#pragma thumb on

extern u8 *gSysSound;
void func_0201c070(void *p, s32 n);
void SysSound_StopBGM(s32 n) { func_0201c070(gSysSound + 0x14, n); }

#pragma thumb reset

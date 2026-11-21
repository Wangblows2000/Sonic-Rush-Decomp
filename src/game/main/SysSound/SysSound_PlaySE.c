#include <nitro/types.h>

#pragma thumb on

extern void *gSysSound;
void SysSound_LoadSEGroup(s32 i);
void SysSound_StartSE(s32 i);
void SysSound_PlaySE(s32 i) { if (gSysSound != NULL && i < 23) { SysSound_LoadSEGroup(i); SysSound_StartSE(i); } }

#pragma thumb reset

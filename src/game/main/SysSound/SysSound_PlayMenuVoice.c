#include <nitro/types.h>

#pragma thumb on

extern void *gSysSound;
void SysSound_LoadVoiceGroup(s32 i);
void SysSound_StartVoice(s32 i);
void SysSound_PlayMenuVoice(s32 i) { if (gSysSound != NULL && i < 8) { SysSound_LoadVoiceGroup(i); SysSound_StartVoice(i); } }

#pragma thumb reset

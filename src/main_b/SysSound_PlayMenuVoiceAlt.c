#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x2c]; u32 unk2C; } UnkTask;
extern UnkTask *gSysSound;
void SysSound_LoadVoiceGroup(s32 i);
void SysSound_StartVoice(s32 i);
void SysSound_PlayMenuVoiceAlt(s32 i) { if (gSysSound != NULL && i < 8) { SysSound_LoadVoiceGroup(i); gSysSound->unk2C = 1; SysSound_StartVoice(i); gSysSound->unk2C = 0; } }

#pragma thumb reset

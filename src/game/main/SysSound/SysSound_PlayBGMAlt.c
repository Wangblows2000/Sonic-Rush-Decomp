#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x28]; u32 unk28; } UnkTask;
extern UnkTask *gSysSound;
void SysSound_LoadBGMGroup(s32 i);
void SysSound_StartBGM(s32 i);
void SysSound_StopAll(s32 i);
void SysSound_PlayBGMAlt(s32 i) { if (gSysSound != NULL) { if (i < 6) { SysSound_LoadBGMGroup(i); gSysSound->unk28 = 1; SysSound_StartBGM(i); gSysSound->unk28 = 0; return; } SysSound_StopAll(i); } }

#pragma thumb reset

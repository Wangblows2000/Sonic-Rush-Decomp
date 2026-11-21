#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; s32 unk4; } UnkTask;
extern UnkTask *gSysSound;
void SysSound_LoadBGMGroup(s32 i);
void SysSound_StartBGM(s32 i);
void SysSound_StopAll(s32 i);
void SysSound_PlayBGM(s32 i) { if (gSysSound != NULL && i != gSysSound->unk4) { if (i < 6) { SysSound_LoadBGMGroup(i); SysSound_StartBGM(i); } else SysSound_StopAll(i); gSysSound->unk4 = i; } }

#pragma thumb reset

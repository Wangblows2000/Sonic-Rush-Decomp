#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 unk4; u32 unk8; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
typedef struct { u8 unk0[0x10]; u32 unk10; } UnkTask2;
extern UnkTask2 *gSysSound;
void SysSound_SetBGMVolume(u8 v);
void func_02051928(UnkTask *t) { SysSound_SetBGMVolume(t->unk10->unk8); if (gSysSound != NULL) gSysSound->unk10 = 0; }

#pragma thumb reset

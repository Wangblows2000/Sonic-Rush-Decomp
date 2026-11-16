#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x2c]; u32 unk2C; } UnkTask;
extern UnkTask *gSysSound;
void func_02051c30(s32 i);
void func_020519b0(s32 i);
void func_020520ac(s32 i) { if (gSysSound != NULL && i < 8) { func_02051c30(i); gSysSound->unk2C = 1; func_020519b0(i); gSysSound->unk2C = 0; } }

#pragma thumb reset

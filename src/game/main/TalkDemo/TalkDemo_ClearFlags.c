#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 unk10[1]; } UnkStruct_02060e38;
typedef struct { u8 unk0[0x10]; UnkStruct_02060e38 *unk10; } UnkTask;
extern UnkTask *gTalkDemoTask;
void TalkDemo_ClearFlags(s32 i, u32 v) { if (gTalkDemoTask != NULL) gTalkDemoTask->unk10->unk10[i] &= ~v; }

#pragma thumb reset

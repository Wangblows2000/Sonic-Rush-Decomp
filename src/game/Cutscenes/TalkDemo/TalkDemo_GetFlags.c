#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 unk10[1]; } UnkStruct_02060e70;
typedef struct { u8 unk0[0x10]; UnkStruct_02060e70 *unk10; } UnkTask;
extern UnkTask *gTalkDemoTask;
u32 TalkDemo_GetFlags(s32 i) { if (gTalkDemoTask == NULL) return 0; return gTalkDemoTask->unk10->unk10[i]; }

#pragma thumb reset

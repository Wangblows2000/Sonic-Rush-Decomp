#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 *unk10; } UnkTask;
extern UnkTask *gTalkDemoTask;
u32 TalkDemo_GetScript(void) { if (gTalkDemoTask == NULL) return 0; return gTalkDemoTask->unk10[9]; }

#pragma thumb reset

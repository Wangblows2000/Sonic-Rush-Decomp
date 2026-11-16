#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x30]; u8 unk30[1]; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gTalkDemoTask;
typedef struct { u32 unk0; void *unk4; u32 unk8; } UnkSub;
typedef struct { u8 unk0[0xd0]; UnkSub sub; } UnkStruct_0206125c;
typedef struct { u32 unk0; u32 unk4; u32 unk8; } UnkInfo;
UnkInfo **TalkDemo_GetEntry(void);
void func_0206125c(UnkStruct_0206125c *s) { UnkSub *p = &s->sub; UnkInfo *i; if (gTalkDemoTask != NULL) p->unk4 = gTalkDemoTask->unk10->unk30; i = *TalkDemo_GetEntry(); if (i != NULL) p->unk8 = i->unk8; }

#pragma thumb reset

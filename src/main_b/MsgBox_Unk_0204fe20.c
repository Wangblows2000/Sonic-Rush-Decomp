#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u8 unk4[0x14]; u32 unk18; } UnkWork;
typedef struct { u32 unk0; u32 unk4; void (*fn)(void); u32 unkC; UnkWork *unk10; } UnkTask;
extern UnkTask *gMsgBoxTask;
void MsgBox_Unk_0204fc60(void);
void MsgBox_Unk_0204fe20(void) { UnkWork *w = gMsgBoxTask->unk10; w->unk0 = 0; w->unk18 = 1; gMsgBoxTask->fn = MsgBox_Unk_0204fc60; }

#pragma thumb reset

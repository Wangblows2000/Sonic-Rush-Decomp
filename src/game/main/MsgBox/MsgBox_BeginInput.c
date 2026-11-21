#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u8 unk4[0x14]; u32 unk18; } UnkWork;
typedef struct { u32 unk0; u32 unk4; void (*fn)(void); u32 unkC; UnkWork *unk10; } UnkTask;
extern UnkTask *gMsgBoxTask;
void MsgBox_StopLineScroll(UnkWork *w);
void MsgBox_Main_Input(void);
void MsgBox_BeginInput(void) { UnkWork *w = gMsgBoxTask->unk10; w->unk0 = 6; w->unk18 = 3; MsgBox_StopLineScroll(w); gMsgBoxTask->fn = MsgBox_Main_Input; }

#pragma thumb reset

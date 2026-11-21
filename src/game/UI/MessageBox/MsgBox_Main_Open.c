#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gTaskCurrent;
void MsgBox_SetSplitScroll(UnkWork *w, u32 a, s32 b, s32 c);
void MsgBox_SetBgVisible(UnkWork *w, s32 a);
void MsgBox_BeginInput(void);
void MsgBox_Main_Open(void) { UnkWork *w = gTaskCurrent->unk10; MsgBox_SetSplitScroll(w, w->unk0, 8, 0x1000); if (w->unk0 == 1) MsgBox_SetBgVisible(w, 1); w->unk0++; if (w->unk0 >= 8) MsgBox_BeginInput(); }

#pragma thumb reset

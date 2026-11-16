#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gTaskCurrent;
void MsgBox_Unk_0204f038(UnkWork *w, u32 a, s32 b, s32 c);
void MsgBox_Unk_0204f2fc(UnkWork *w, s32 a);
void MsgBox_Unk_0204fd9c(void);
void MsgBox_Unk_0204fa34(void) { UnkWork *w = gTaskCurrent->unk10; MsgBox_Unk_0204f038(w, w->unk0, 8, 0x1000); if (w->unk0 == 1) MsgBox_Unk_0204f2fc(w, 1); w->unk0++; if (w->unk0 >= 8) MsgBox_Unk_0204fd9c(); }

#pragma thumb reset

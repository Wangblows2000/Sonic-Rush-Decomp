#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x24]; u32 unk24; u8 unk28[0x498]; u8 unk4C0[0x90]; u32 unk550; } UnkWork;
typedef struct { u32 unk0; u32 unk4; void (*fn)(void); u32 unkC; UnkWork *unk10; } UnkTask;
extern UnkTask *gTaskCurrent;
void MsgBox_InitSprite(UnkWork *w, void *a, u32 b, s32 c, s32 d);
void MsgBox_Main_InitMessage(void);
void MsgBox_Main_InitCursor(void) { UnkWork *w = gTaskCurrent->unk10; if (w->unk24 == 1) MsgBox_InitSprite(w, w->unk4C0, w->unk550, 0, 1); gTaskCurrent->fn = MsgBox_Main_InitMessage; }

#pragma thumb reset

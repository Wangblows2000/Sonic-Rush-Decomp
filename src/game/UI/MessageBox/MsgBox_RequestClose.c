#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 *unk10; } UnkTask;
extern UnkTask *gMsgBoxTask;
void MsgBox_RequestClose(void) { if (gMsgBoxTask != NULL) gMsgBoxTask->unk10[0x12] = 1; }

#pragma thumb reset

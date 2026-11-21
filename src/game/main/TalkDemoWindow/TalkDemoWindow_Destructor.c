#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; void *unk10; } UnkTask;
extern void *data_022c4d9c;
void TalkDemoWindow_CancelVAlarms(void *p);
void TalkDemoWindow_Destructor(UnkTask *t) { TalkDemoWindow_CancelVAlarms(t->unk10); data_022c4d9c = NULL; }

#pragma thumb reset

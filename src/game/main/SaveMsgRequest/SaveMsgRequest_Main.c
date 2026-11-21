#include <nitro/types.h>

typedef struct { u8 unk0[0x14]; void *unk14; void *unk18; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gTaskCurrent;
void Task_DestroyAll(void);
void SaveMsg_Create(void *a, void *b);
void SaveMsgRequest_Main(void) { UnkWork *w = gTaskCurrent->unk10; void *a = w->unk14; void *b = w->unk18; Task_DestroyAll(); SaveMsg_Create(a, b); }

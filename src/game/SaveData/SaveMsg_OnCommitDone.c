#include <nitro/types.h>

typedef struct { u8 unk0[8]; u32 flags; u8 unkC[0xc]; void (*cb)(void); } UnkStruct_02059f24;
extern void *gTaskCurrent;
void Task_Destroy(void *p);
void SaveMsg_Create(s32 a, s32 b);
void SaveMsg_OnCommitDone(UnkStruct_02059f24 *s) { void (*cb)(void); if (s->flags & 1) { Task_Destroy(gTaskCurrent); SaveMsg_Create(2, 0); return; } cb = s->cb; Task_Destroy(gTaskCurrent); if (cb != NULL) cb(); }

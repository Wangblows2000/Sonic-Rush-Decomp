#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; void (*cb)(void); } UnkStruct_02059d7c;
extern void *gTaskCurrent;
void Task_Destroy(void *p);
void SaveMsg_OnFormatDone(UnkStruct_02059d7c *s) { void (*cb)(void) = s->cb; Task_Destroy(gTaskCurrent); if (cb != NULL) cb(); }

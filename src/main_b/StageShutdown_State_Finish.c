#include <nitro/types.h>

typedef struct { void (*cb)(void); } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gTaskCurrent;
void Task_Destroy(void *p);
void StageShutdown_State_Finish(void) { void (*cb)(void) = gTaskCurrent->unk10->cb; Task_Destroy(gTaskCurrent); if (cb != NULL) cb(); }

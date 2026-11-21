#include <nitro/types.h>

typedef struct { u32 unk0; void *tasks[2]; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gNaviBTask;
void Task_Destroy(void *p);
void NaviB_Destroy(void) { s32 i; UnkWork *w = gNaviBTask->unk10; for (i = 0; i < 2; i++) { if (w->tasks[i] != NULL) Task_Destroy(w->tasks[i]); } Task_Destroy(gNaviBTask); }

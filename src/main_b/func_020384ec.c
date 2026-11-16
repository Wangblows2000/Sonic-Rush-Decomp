#include <nitro/types.h>

typedef struct { u32 unk0; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gTaskCurrent;
extern u32 gSystemFlags;
void Task_Destroy(void *p);
void func_020384ec(void) { if (gTaskCurrent->unk10->unk0 != 0) gSystemFlags |= 0x8000000; else gSystemFlags &= ~0x8000000; Task_Destroy(gTaskCurrent); }

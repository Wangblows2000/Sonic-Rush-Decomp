#include <nitro/types.h>

typedef struct { u8 unk0[0x14]; void *unk14; void *unk18; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gTaskCurrent;
void Task_DestroyAll(void);
void func_0205a148(void *a, void *b);
void func_0205a23c(void) { UnkWork *w = gTaskCurrent->unk10; void *a = w->unk14; void *b = w->unk18; Task_DestroyAll(); func_0205a148(a, b); }

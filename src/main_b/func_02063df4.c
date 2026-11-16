#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; void *unk10; } UnkTask;
extern UnkTask *gTaskCurrent;
void func_02063cb4(void *p);
void func_02063df4(void) { func_02063cb4(gTaskCurrent->unk10); }

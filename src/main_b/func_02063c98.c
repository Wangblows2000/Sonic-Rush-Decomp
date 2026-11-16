#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; void *unk10; } UnkTask;
extern UnkTask *gTaskCurrent;
void func_02063890(void *p);
void func_02063c98(void) { func_02063890(gTaskCurrent->unk10); }

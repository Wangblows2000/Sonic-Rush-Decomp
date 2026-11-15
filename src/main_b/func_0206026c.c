#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; void *unk10; } UnkTask;
extern void *data_022c4d9c;
void func_0205fd50(void *p);
void func_0206026c(UnkTask *t) { func_0205fd50(t->unk10); data_022c4d9c = NULL; }

#pragma thumb reset

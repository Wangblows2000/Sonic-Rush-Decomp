#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; void *unk10; } UnkTask;
extern UnkTask *data_022c467c;
void *func_02058350(void) { if (data_022c467c != NULL) return data_022c467c->unk10; return NULL; }

#pragma thumb reset

#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 unk4; u32 unk8; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
typedef struct { u8 unk0[0x10]; u32 unk10; } UnkTask2;
extern UnkTask2 *data_022c465c;
void func_02051e90(u8 v);
void func_02051928(UnkTask *t) { func_02051e90(t->unk10->unk8); if (data_022c465c != NULL) data_022c465c->unk10 = 0; }

#pragma thumb reset

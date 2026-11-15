#include <nitro/types.h>

typedef struct { u8 unk0[8]; void *unk8; } UnkC;
typedef struct { u8 unk0[8]; UnkC *unk8; } UnkB;
typedef struct { u8 unk0[0x10]; UnkB *unk10; } UnkTask;
extern UnkTask *data_022c4dc4;
void func_02063d34(void);
BOOL func_02064758(void) { return data_022c4dc4->unk10->unk8->unk8 == (void *)func_02063d34; }

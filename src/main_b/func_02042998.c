#include <nitro/types.h>

typedef struct { u8 unk0[0x14]; u16 unk14; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022c4364;
void func_02042904(u16 v);
void func_02042998(void) { func_02042904(data_022c4364->unk10->unk14); }

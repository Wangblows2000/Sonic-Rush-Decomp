#include <nitro/types.h>

typedef struct { u32 unk0; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022b4574;
extern u32 data_02087cb8;
void func_020371c8(void *p);
void func_020384ec(void) { if (data_022b4574->unk10->unk0 != 0) data_02087cb8 |= 0x8000000; else data_02087cb8 &= ~0x8000000; func_020371c8(data_022b4574); }

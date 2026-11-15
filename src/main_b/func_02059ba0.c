#include <nitro/types.h>

typedef struct { u8 unk0[8]; u32 unk8; } UnkStruct_022c4680;
extern UnkStruct_022c4680 data_022c4680;
void func_02059ba0(BOOL on) { if (on) data_022c4680.unk8 &= ~1; else data_022c4680.unk8 |= 1; }

#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x14]; u16 unk14; } UnkStruct_022c4644;
extern UnkStruct_022c4644 data_022c4644;
extern u32 data_022c4640;
u16 func_02051300(void *p, s32 n);
void func_02051384(void) { data_022c4644.unk14 = func_02051300(&data_022c4644, 10); data_022c4640 = 1; }

#pragma thumb reset

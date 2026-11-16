#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; u32 unk18; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
void func_0204bc4c(void);
void *Task_Create(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void func_0204d63c(void) { data_022c4560.unk18 |= 2; Task_Create(func_0204bc4c, 0, 0, 0, 0, 0); }

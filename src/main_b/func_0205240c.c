#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x1c]; u8 unk1C; u8 unk1D; u16 unk1E; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
void func_0204884c(void);
void func_0205240c(void) { data_022c4560.unk1C = 8; data_022c4560.unk1D = 2; func_0204884c(); }

#pragma thumb reset

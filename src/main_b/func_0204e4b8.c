#include <nitro/types.h>

typedef struct { u8 unk0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
void func_0204d6d0(s32 a);
void func_0204e4b8(void) { data_022c4560.unk1D++; func_0204d6d0(3); }

#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x1c]; u8 unk1C; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
void func_02048734(s32 a);
void func_02048864(void) { if (data_022c4560.unk1C == 8) func_02048734(2); else func_02048734(1); }

#pragma thumb reset

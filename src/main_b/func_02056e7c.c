#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 unk4; } UnkStruct_02076544;
extern UnkStruct_02076544 data_02076544[];
extern s16 data_022c4678;
UnkStruct_02076544 *func_02056e7c(void) { return &data_02076544[data_022c4678]; }

#pragma thumb reset

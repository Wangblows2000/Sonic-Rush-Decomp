#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 unk10; } UnkStruct_022b65b4;
extern UnkStruct_022b65b4 data_022b65b4;
BOOL func_0204f43c(void) { if (data_022b65b4.unk10 & 0x40) return TRUE; return FALSE; }

#pragma thumb reset

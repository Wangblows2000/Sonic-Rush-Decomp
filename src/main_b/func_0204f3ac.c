#include <nitro/types.h>

#pragma thumb on

typedef struct { u16 x; u16 y; u8 unk4[4]; u16 unk8; u16 unkA; u8 unkC[4]; u32 unk10; } UnkStruct_022b65b4;
extern UnkStruct_022b65b4 data_022b65b4;
BOOL func_0204f3ac(void) { u16 x, y; if (!(data_022b65b4.unk10 & 0x40)) return FALSE; x = data_022b65b4.unk8; y = data_022b65b4.unkA; if (x >= 0x84 && x < 0xbc && y >= 0x70 && y < 0x90) return TRUE; return FALSE; }

#pragma thumb reset

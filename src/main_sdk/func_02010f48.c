#include <nitro/types.h>

s32 func_02010d60(u32 a, u32 b, u32 c);
typedef struct { u8 pad[0x16]; u16 x16; } UnkStruct_02010f48;
extern UnkStruct_02010f48 *data_0207faec;
s32 func_02010f48(u32 a, u32 b) { s32 r = func_02010d60(a, b, 0x1700); if (r == 0) data_0207faec->x16 = 0; return r; }

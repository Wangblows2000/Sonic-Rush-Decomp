#include <nitro/types.h>

u32 MATH_CountPopulation(u32 v);
typedef struct { u8 pad[0x810]; u16 x810; } UnkStruct_02011f4c;
u32 WmGetSharedDataAddress(UnkStruct_02011f4c *p, u32 a, u32 b, u32 n) { return MATH_CountPopulation(a & ((1 << n) - 1)) * p->x810 + b; }

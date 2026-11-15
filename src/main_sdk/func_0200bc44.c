#include <nitro/types.h>

typedef struct { u8 data[0x44]; } UnkStruct_0200bc44;
void func_0200be70(UnkStruct_0200bc44 *p);
s32 func_0200bd10(UnkStruct_0200bc44 *p, u32 a, u32 b, u32 c);
BOOL func_0200bc44(u32 b, u32 a) { UnkStruct_0200bc44 s; func_0200be70(&s); return func_0200bd10(&s, a, b, 0) ? 1 : 0; }

#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[0x16]; u16 unk1A; } UnkStruct_02041a04;
extern u32 data_02087cb8;
u32 func_0203e934(void);
void func_020419e4(void);
void func_02041a04(UnkStruct_02041a04 *s) { if (func_0203e934() != 5) return; data_02087cb8 |= 0x100; data_02087cb8 &= ~0x200; s->unk1A = 0; s->fn = func_020419e4; }

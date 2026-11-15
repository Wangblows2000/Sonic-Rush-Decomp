#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[0x16]; u16 unk1A; } UnkStruct_020421d4;
extern u32 data_02087cb8;
u32 func_0203e934(void);
void func_020421b4(void);
void func_020421d4(UnkStruct_020421d4 *s) { func_0203e934(); if (func_0203e934() != 5) return; data_02087cb8 |= 0x300; s->unk1A = 0; s->fn = func_020421b4; }

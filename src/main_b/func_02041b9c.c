#include <nitro/types.h>

typedef struct { void (*fn)(void); u32 unk4; u32 unk8; u8 unkC[0xe]; u16 unk1A; u8 unk1C[0x190e]; u16 unk192A; } UnkStruct_02041b9c;
void func_02041b00(void);
void func_02041b9c(UnkStruct_02041b9c *s) { s->unk8 = 4; s->unk192A = 0xffff; s->unk1A = 0; s->fn = func_02041b00; }

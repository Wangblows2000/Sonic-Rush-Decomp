#include <nitro/types.h>

typedef struct { void (*fn)(void); u32 unk4; u32 unk8; u8 unkC[0xe]; u16 unk1A; } UnkStruct_020421b4;
void func_0204211c(void);
void func_020421b4(UnkStruct_020421b4 *s) { s->unk8 = 1; s->unk1A = 0; s->fn = func_0204211c; }

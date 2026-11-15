#include <nitro/types.h>

typedef struct { void (*fn)(void); u32 unk4; u32 unk8; u8 unkC[0xe]; u16 unk1A; } UnkStruct_02042304;
void func_020422a4(void);
void func_02042304(UnkStruct_02042304 *s) { s->unk8 = 1; s->unk1A = 0; s->fn = func_020422a4; }

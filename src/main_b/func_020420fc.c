#include <nitro/types.h>

typedef struct { void (*fn)(void); u32 unk4; u32 unk8; u8 unkC[0xe]; u16 unk1A; } UnkStruct_020420fc;
void func_02042080(void);
void func_020420fc(UnkStruct_020420fc *s) { s->unk8 = 2; s->unk1A = 0; s->fn = func_02042080; }

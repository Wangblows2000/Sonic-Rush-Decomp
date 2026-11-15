#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u32 unk4; u32 unk8; u32 unkC; } UnkStruct_020626e8;
void func_020626d0(void);
void func_020626e8(UnkStruct_020626e8 *s) { s->unkC = 10; s->fn = func_020626d0; }

#pragma thumb reset

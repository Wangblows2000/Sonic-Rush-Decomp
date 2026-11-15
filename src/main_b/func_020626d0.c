#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u32 unk4; u32 unk8; s32 unkC; } UnkStruct_020626d0;
void func_0206267c(void);
void func_020626d0(UnkStruct_020626d0 *s) { s->unkC--; if (s->unkC <= 0) s->fn = func_0206267c; }

#pragma thumb reset

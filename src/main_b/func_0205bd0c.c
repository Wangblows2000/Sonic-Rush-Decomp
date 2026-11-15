#include <nitro/types.h>

typedef struct UnkObj_0205bd0c { void (*fn)(void *, struct UnkObj_0205bd0c *); u8 unk4[0xa0]; s32 unkA4; s32 unkA8; s32 unkAC; s32 unkB0; u8 unkB4[0xc]; s16 unkC0; } UnkObj_0205bd0c;
void func_0205bc74(void *a, UnkObj_0205bd0c *o);
void func_0205bd0c(void *a, UnkObj_0205bd0c *o) { o->unkAC = o->unkA4; o->unkB0 = o->unkA8; o->unkC0 = 0; o->fn = func_0205bc74; o->fn(a, o); }

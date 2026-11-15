#include <nitro/types.h>

typedef struct UnkObj_0205c67c { void (*fn)(void *, struct UnkObj_0205c67c *); u8 unk4[8]; u16 unkC; } UnkObj_0205c67c;
void func_0205c624(void *a, UnkObj_0205c67c *o);
void func_0205c67c(void *a, UnkObj_0205c67c *o) { o->unkC = 0; o->fn = func_0205c624; o->fn(a, o); }

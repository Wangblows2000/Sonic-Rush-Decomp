#include <nitro/types.h>

typedef struct UnkObj_0205caa0 { void (*fn)(void *, struct UnkObj_0205caa0 *); } UnkObj_0205caa0;
void func_0205ca64(void *a, UnkObj_0205caa0 *o);
void func_0205caa0(void *a, UnkObj_0205caa0 *o) { o->fn = func_0205ca64; o->fn(a, o); }

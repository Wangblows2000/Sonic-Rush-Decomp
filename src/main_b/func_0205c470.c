#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[0x9a]; s16 unk9E; } UnkObj_0205c470;
void func_0205c3a0(void);
void func_0205c470(void *a, UnkObj_0205c470 *o) { if (o->unk9E++ > 40) { o->unk9E = 0; o->fn = func_0205c3a0; } }

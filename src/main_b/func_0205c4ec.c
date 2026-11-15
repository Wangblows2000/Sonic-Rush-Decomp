#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[0x48]; s32 unk4C; u32 unk50; s32 unk54; } UnkObj_0205c4ec;
void func_0205c4dc(void);
void func_0205c4ec(void *a, UnkObj_0205c4ec *o) { o->unk54 += 0x100; if (o->unk54 > 0x8000) o->unk54 = 0x8000; o->unk4C -= o->unk54; if (o->unk4C <= -0x100000) o->fn = func_0205c4dc; }

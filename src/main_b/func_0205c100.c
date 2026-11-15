#include <nitro/types.h>

typedef struct UnkObj_0205c100 { void (*fn)(void *, struct UnkObj_0205c100 *); u8 unk4[0x48]; s32 unk4C; s32 unk50; s32 unk54; s32 unk58; u8 unk5C[0x12]; s16 unk6E; } UnkObj_0205c100;
void func_0205c048(void *a, UnkObj_0205c100 *o);
void func_0205c100(void *a, UnkObj_0205c100 *o) { o->unk54 = o->unk4C; o->unk58 = o->unk50; o->unk6E = 0; o->fn = func_0205c048; o->fn(a, o); }

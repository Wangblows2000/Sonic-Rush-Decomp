#include <nitro/types.h>

typedef struct { u8 unk0[0x48c]; u32 flags2; } UnkStruct_0205ca64;
typedef struct UnkObj_0205ca64 { void (*fn)(UnkStruct_0205ca64 *, struct UnkObj_0205ca64 *); u8 unk4[8]; u16 unkC; } UnkObj_0205ca64;
void func_0205c794(UnkStruct_0205ca64 *a, UnkObj_0205ca64 *o);
void func_0205ca64(UnkStruct_0205ca64 *s, UnkObj_0205ca64 *o) { s->flags2 |= 0x400; o->unkC = 0; o->fn = func_0205c794; o->fn(s, o); }

#include <nitro/types.h>

typedef struct { u8 unk0[0x48c]; u32 flags2; } UnkStruct_0205c708;
typedef struct UnkObj_0205c708 { void (*fn)(UnkStruct_0205c708 *, struct UnkObj_0205c708 *); u8 unk4[0x48]; s32 unk4C; s32 unk50; } UnkObj_0205c708;
void func_0205c704(UnkStruct_0205c708 *a, UnkObj_0205c708 *o);
void func_0205c708(UnkStruct_0205c708 *s, UnkObj_0205c708 *o) { s->flags2 |= 2; o->unk4C = 0; o->unk50 = 0; o->fn = func_0205c704; o->fn(s, o); }

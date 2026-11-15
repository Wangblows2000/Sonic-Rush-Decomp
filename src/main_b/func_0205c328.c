#include <nitro/types.h>

typedef struct { u8 unk0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
extern u32 data_02076b90[];
typedef struct { u8 unk0[0x488]; u32 flags; u32 flags2; } UnkStruct_0205c328;
typedef struct UnkObj_0205c328 { void (*fn)(UnkStruct_0205c328 *, struct UnkObj_0205c328 *); u8 unk4[0x58]; u32 unk5C; u32 unk60; u8 unk64[0xa]; s16 unk6E; } UnkObj_0205c328;
void func_0205c164(UnkStruct_0205c328 *s, UnkObj_0205c328 *o);
void func_0205c328(UnkStruct_0205c328 *s, UnkObj_0205c328 *o) { s->flags &= ~0x100; s->flags2 |= 0x10; o->unk5C = data_02076b90[data_022c4560.unk1C]; o->unk60 = 0; o->unk6E = 0; o->fn = func_0205c164; o->fn(s, o); }

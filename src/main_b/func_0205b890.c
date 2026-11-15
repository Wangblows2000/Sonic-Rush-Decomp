#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; u32 flags2; } UnkStruct_0205b890;
typedef struct UnkObj_0205b890 { void (*fn)(UnkStruct_0205b890 *, struct UnkObj_0205b890 *); void *unk4; u16 unk8; u8 unkA[0xe]; u16 unk18; } UnkObj_0205b890;
extern u8 data_02076b68;
void func_0205b75c(UnkStruct_0205b890 *s, UnkObj_0205b890 *o);
void func_0205b890(UnkStruct_0205b890 *s, UnkObj_0205b890 *o) { s->flags &= ~0x4000; s->flags2 |= 0x80; o->unk4 = &data_02076b68; o->unk8 = 12; o->unk18 = 0; o->fn = func_0205b75c; o->fn(s, o); }

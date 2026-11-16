#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; u32 flags2; } UnkStruct_0205b6f0;
typedef struct UnkObj_0205b6f0 { void (*fn)(UnkStruct_0205b6f0 *, struct UnkObj_0205b6f0 *); void *unk4; u16 unk8; u8 unkA[0xe]; u16 unk18; } UnkObj_0205b6f0;
extern u8 data_02076b54;
void StageTitleCardText404_State_Enter(UnkStruct_0205b6f0 *s, UnkObj_0205b6f0 *o);
void StageTitleCardText404_State_Init(UnkStruct_0205b6f0 *s, UnkObj_0205b6f0 *o) { s->flags &= ~0x10000; s->flags2 |= 0x100; o->unk4 = &data_02076b54; o->unk8 = 6; o->unk18 = 0; o->fn = StageTitleCardText404_State_Enter; o->fn(s, o); }

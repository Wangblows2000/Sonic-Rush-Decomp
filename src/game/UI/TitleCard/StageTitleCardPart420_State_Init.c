#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; u32 flags2; } UnkStruct_0205b574;
typedef struct { void (*fn)(void); u8 unk4[0x48]; s32 unk4C; s32 unk50; } UnkObj_0205b574;
void StageTitleCardPart420_State_Done(void);
void StageTitleCardPart420_State_Init(UnkStruct_0205b574 *s, UnkObj_0205b574 *o) { s->flags &= ~0x40000; s->flags2 |= 0x200; o->unk4C = 0x80000; o->unk50 = 0x46000; o->fn = StageTitleCardPart420_State_Done; }

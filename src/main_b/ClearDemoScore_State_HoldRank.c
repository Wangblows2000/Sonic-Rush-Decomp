#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x344]; u32 flags; } UnkStruct_0204622c;
typedef struct { void (*fn)(void); s32 unk4; } UnkObj_0204622c;
void ClearDemo_Unk_02046228(void);
void ClearDemoScore_State_HoldRank(UnkStruct_0204622c *s, UnkObj_0204622c *o) { o->unk4++; if (o->unk4 >= 60) { s->flags |= 0x8000; o->fn = ClearDemo_Unk_02046228; o->unk4 = 0; } }

#pragma thumb reset

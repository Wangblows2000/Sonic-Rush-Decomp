#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x344]; u32 flags; } UnkStruct_02045598;
typedef struct { void (*fn)(void); s32 unk4; } UnkObj_02045598;
void ClearDemoTA_State_Done(void);
void ClearDemoTA_State_HoldRank(UnkStruct_02045598 *s, UnkObj_02045598 *o) { o->unk4++; if (o->unk4 >= 60) { s->flags |= 0x8000; o->fn = ClearDemoTA_State_Done; o->unk4 = 0; } }

#pragma thumb reset

#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x344]; u32 flags; } UnkStruct_02046ebc;
typedef struct { void (*fn)(void); u8 unk4[0x62]; u16 unk66; } UnkObj_02046ebc;
void ClearDemo_Unk_02046ea4(void);
void ClearDemo_Unk_02046ebc(UnkStruct_02046ebc *s, UnkObj_02046ebc *o) { o->unk66++; if (o->unk66 > 30) { o->fn = ClearDemo_Unk_02046ea4; s->flags |= 0x3000; } }

#pragma thumb reset

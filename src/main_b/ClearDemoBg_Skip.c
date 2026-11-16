#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); s32 unk4; s32 unk8; u8 unkC[0x338]; u32 flags; } UnkStruct_02047274;
typedef struct { u8 unk0[0x1c]; u32 unk1C; } UnkStruct_0208922c;
extern UnkStruct_0208922c gDisplayMain;
void ClearDemoBg_State_Done(void);
void ClearDemoBg_Skip(UnkStruct_02047274 *s) { s->flags |= 0x18; gDisplayMain.unk1C = 0; s->unk4 = 0xdf000; s->unk8 = -0xa0000; s->fn = ClearDemoBg_State_Done; }

#pragma thumb reset

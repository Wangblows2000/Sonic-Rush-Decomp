#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 flags; } UnkSub;
typedef struct { void (*fn)(void); UnkSub *unk4; } UnkStruct_02056d2c;
void PlayerDemoBg_Unk_02056b80(void);
void PlayerDemoBg_Unk_02056cf8(void);
void PlayerDemoBg_Unk_02056d2c(UnkStruct_02056d2c *s) { if (s->unk4->flags & 0x20) { s->unk4->flags &= ~0x20; PlayerDemoBg_Unk_02056b80(); s->fn = PlayerDemoBg_Unk_02056cf8; } }

#pragma thumb reset

#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 flags; } UnkSub;
typedef struct { void (*fn)(void); UnkSub *unk4; u8 unk8[4]; s8 unkC; s8 unkD; } UnkStruct_02056c9c;
void PlayerDemoBg_Unk_02056c98(void);
void PlayerDemoBg_Unk_02056d54(void);
void PlayerDemoBg_Unk_02056c9c(UnkStruct_02056c9c *s) { UnkSub *p = s->unk4; if (p->flags & 0x10) { p->flags &= ~0x10; s->unkC++; if (s->unkC >= s->unkD) { s->fn = PlayerDemoBg_Unk_02056c98; return; } s->fn = PlayerDemoBg_Unk_02056d54; } }

#pragma thumb reset

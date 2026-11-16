#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 flags; } UnkSub;
typedef struct { void (*fn)(void); UnkSub *unk4; u8 unk8[0x4c]; u16 unk54; u16 unk56; } UnkStruct_02056cf8;
void PlayerDemoBg_State_Hide(void);
void PlayerDemoBg_State_WaitHide(UnkStruct_02056cf8 *s) { UnkSub *p = s->unk4; if (p->flags & 0x40) { p->flags &= ~0x40; s->unk54 = 6; s->unk56 = 6; s->fn = PlayerDemoBg_State_Hide; } }

#pragma thumb reset

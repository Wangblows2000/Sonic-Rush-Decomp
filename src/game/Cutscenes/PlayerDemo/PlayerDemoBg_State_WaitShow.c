#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 flags; } UnkSub;
typedef struct { void (*fn)(void); UnkSub *unk4; } UnkStruct_02056d2c;
void PlayerDemoBg_ShowImage(void);
void PlayerDemoBg_State_WaitHide(void);
void PlayerDemoBg_State_WaitShow(UnkStruct_02056d2c *s) { if (s->unk4->flags & 0x20) { s->unk4->flags &= ~0x20; PlayerDemoBg_ShowImage(); s->fn = PlayerDemoBg_State_WaitHide; } }

#pragma thumb reset

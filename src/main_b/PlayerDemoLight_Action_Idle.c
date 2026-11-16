#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 flags; } UnkSub;
typedef struct { u32 unk0; UnkSub *unk4; } UnkStruct_020560d0;
typedef struct { void (*fn)(void); u32 unk4; u32 flags; } UnkObj_020560d0;
void PlayerDemoLight_Apply(void *a);
void PlayerDemoLight_State_Idle(void);
void PlayerDemoLight_Action_Idle(UnkStruct_020560d0 *s, UnkObj_020560d0 *o) { o->flags &= ~1; PlayerDemoLight_Apply(s); s->unk4->flags &= ~0x2000; o->fn = PlayerDemoLight_State_Idle; }

#pragma thumb reset

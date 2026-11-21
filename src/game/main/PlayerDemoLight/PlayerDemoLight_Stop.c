#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 flags; } UnkSub;
typedef struct { u32 unk0; UnkSub *unk4; } UnkStruct_02056118;
void PlayerDemoLight_Apply(void *a);
void PlayerDemoLight_Stop(UnkStruct_02056118 *s, u32 *b) { PlayerDemoLight_Apply(s); *b = 0; s->unk4->flags &= ~0x7000; }

#pragma thumb reset

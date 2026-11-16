#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 flags; } UnkStruct_02057f38;
void PlayerDemo_StartFade(void *a, s32 b);
void PlayerDemoOp_FadeFromWhite(UnkStruct_02057f38 *s, void *b) { if (!(s->flags & 0x800000)) PlayerDemo_StartFade(b, 4); }

#pragma thumb reset

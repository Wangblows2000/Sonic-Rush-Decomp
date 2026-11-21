#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 flags; } UnkStruct_02057f70;
void PlayerDemo_StartFade(void *a, s32 b);
void PlayerDemoOp_FadeFromBlackMain(UnkStruct_02057f70 *s, void *b) { if (!(s->flags & 0x800000)) PlayerDemo_StartFade(b, 1); }

#pragma thumb reset

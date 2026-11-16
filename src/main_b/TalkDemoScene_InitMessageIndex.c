#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 flags; } UnkStruct_020610b8;
s32 TalkDemo_GetMessageIndex(UnkStruct_020610b8 *s);
BOOL TalkDemoScene_InitMessageIndex(UnkStruct_020610b8 *s, s16 *out) { s32 r = TalkDemo_GetMessageIndex(s); if (r < 0) { s->flags |= 0x20; return FALSE; } *out = r; return TRUE; }

#pragma thumb reset

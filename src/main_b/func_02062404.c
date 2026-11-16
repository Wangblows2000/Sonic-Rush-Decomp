#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0x7a]; u16 unk7E; u16 unk80; } UnkStruct_02062404;
void TalkDemo_SetFlags(s32 i, u32 v);
void func_02062404(UnkStruct_02062404 *s) { s->unk7E++; if (s->unk7E >= s->unk80) { TalkDemo_SetFlags(3, 0x100); s->fn = NULL; } }

#pragma thumb reset

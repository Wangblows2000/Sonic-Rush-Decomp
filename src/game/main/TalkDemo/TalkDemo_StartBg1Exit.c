#include <nitro/types.h>

#pragma thumb on

typedef struct { s32 unk0; s32 unk4; s32 unk8; } UnkStruct_0206195c;
BOOL TalkDemo_SetBg1Scroll(UnkStruct_0206195c *s);
BOOL TalkDemo_StartBg1Exit(UnkStruct_0206195c *s, u32 mode) { switch (mode) { case 0: case 1: case 3: default: s->unk4 = -0xb000; s->unk8 = -0x5000; s->unk0 = 0; break; case 2: s->unk0 = 0; break; } TalkDemo_SetBg1Scroll(s); return TRUE; }

#pragma thumb reset

// cc: 1.2/sp2 (does not match default 1.2/sp3)
#include <nitro/types.h>

#pragma thumb on

typedef struct { s32 unk0; s32 unk4; s32 unk8; } UnkStruct_0206195c;
BOOL func_02061940(UnkStruct_0206195c *s);
BOOL func_0206195c(UnkStruct_0206195c *s, u32 mode) { switch (mode) { case 0: case 1: case 3: default: s->unk4 = -0xb000; s->unk8 = -0x5000; s->unk0 = 0; break; case 2: s->unk0 = 0; break; } func_02061940(s); return TRUE; }

#pragma thumb reset

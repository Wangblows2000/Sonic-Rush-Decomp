#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 flags; } UnkStruct_02040cf4;
BOOL func_02040cf4(UnkStruct_02040cf4 *s) { if (s->flags & 2) return FALSE; if (s->flags & 0x40) return TRUE; return FALSE; }

#pragma thumb reset

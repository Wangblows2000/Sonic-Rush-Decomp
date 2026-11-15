#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 flags; } UnkStruct_02057f70;
void func_020572b4(void *a, s32 b);
void func_02057f70(UnkStruct_02057f70 *s, void *b) { if (!(s->flags & 0x800000)) func_020572b4(b, 1); }

#pragma thumb reset
